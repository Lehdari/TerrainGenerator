#include "ErosionSimulator.hpp"

#include <sstream>


float ErosionSimulator::quad__[] = {
    -1.0f,  -1.0f,  0.0f,
    1.0f,   -1.0f,  0.0f,
    -1.0f,  1.0f,   0.0f,
    -1.0f,  1.0f,   0.0f,
    1.0f,   -1.0f,  0.0f,
    1.0f,   1.0f,   0.0f
};


Chunk::Chunk(void) :
    dataTexture_{ { 0, 0 } }
{}

Chunk::Chunk(const sf::Image& heightMap, float hmMin, float hmMax) :
    dataTexture_{ { 0, 0 } }
{
    const unsigned width = heightMap.getSize().x;
    const unsigned height = heightMap.getSize().y;
    const unsigned long dataSize = width * height * 4;

    //  initialize data textures
    for (auto i=0u; i<N_CHUNK_DATATEXTURES; ++i) {
        for (auto j=0u; j<2; ++j) {
            glGenTextures(1, &dataTexture_[i][j]);
            glBindTexture(GL_TEXTURE_2D, dataTexture_[i][j]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        }
    }

    //  upload data
    const uint8_t* hmPix = heightMap.getPixelsPtr();
    std::vector<float> hmData;
    hmData.resize(dataSize, 0.0f);
    //  extract only the red component of heightmap
    for (auto i=0ul; i<dataSize; i+=4)
        hmData[i] = hmMin + (hmPix[i]/255.0f)*(hmMax-hmMin);

    glBindTexture(GL_TEXTURE_2D, dataTexture_[0][0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, &hmData[0]);
}

Chunk::~Chunk(void) {
    for (auto i=0u; i<N_CHUNK_DATATEXTURES; ++i) {
        if (dataTexture_[i][0])
            glDeleteTextures(1, &dataTexture_[i][0]);
        if (dataTexture_[i][1])
            glDeleteTextures(1, &dataTexture_[i][1]);
    }
}

Chunk::Chunk(Chunk&& other) {
    *this = std::move(other);
}

Chunk& Chunk::operator=(Chunk&& other) {
    for (auto i=0u; i<N_CHUNK_DATATEXTURES; ++i) {
        dataTexture_[i][0] = other.dataTexture_[i][0];
        dataTexture_[i][1] = other.dataTexture_[i][1];
        other.dataTexture_[i][0] = 0;
        other.dataTexture_[i][1] = 0;
    }

    return *this;
}


ErosionSimulator::ErosionSimulator(Shader& computeShader,
                                   unsigned blockSizeX, unsigned blockSizeY,
                                   unsigned nBlocksX, unsigned nBlocksY,
                                   const std::string& heightMapFileNamePrefix) :
    computeShader_(computeShader),
    blockSizeX_(blockSizeX), blockSizeY_(blockSizeY),
    nBlocksX_(nBlocksX), nBlocksY_(nBlocksY),
    heightMapFileNamePrefix_(heightMapFileNamePrefix),
    vertexArrayId_(0), vertexBufferId_(0),
    framebufferId_(0),
    activeTextures_(0)
{
    //  vertex array
    glGenVertexArrays(1, &vertexArrayId_);
    glBindVertexArray(vertexArrayId_);

    //  vertex buffer for the quad
    glGenBuffers(1, &vertexBufferId_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad__), quad__, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);

    //  framebuffer
    glGenFramebuffers(1, &framebufferId_);

    for (auto i=0u; i<N_CHUNK_DATATEXTURES; ++i)
        drawBuffers_[i] = GL_COLOR_ATTACHMENT0 + i;
}

ErosionSimulator::~ErosionSimulator(void) {
    if (framebufferId_)
        glDeleteFramebuffers(1, &framebufferId_);
    if (vertexBufferId_)
        glDeleteBuffers(1, &vertexBufferId_);
    if (vertexArrayId_)
        glDeleteVertexArrays(1, &vertexArrayId_);
}

void ErosionSimulator::erode(Shader& visShader) {
    sf::Image img;

    chunks_.clear();

    //  first 2 rows of chunks
    {
        std::vector<Chunk> col;
        col.resize(nBlocksY_);
        chunks_.push_back(std::move(col));
    }

    {
        std::vector<Chunk> col;
        for (auto j=0u; j<nBlocksY_; ++j) {
            std::stringstream hmSs;
            hmSs << heightMapFileNamePrefix_ << "0_" << j << ".png";
            img.loadFromFile(hmSs.str());
            col.emplace_back(img, 0.5f, 1.0f);
        }
        chunks_.push_back(std::move(col));
    }

    for (auto i=0u; i<nBlocksX_; ++i) {
        //  load new chunks
        std::vector<Chunk> col;

        if (i>0)
            chunks_.pop_front();

        if (i+1 < nBlocksX_) {
            for (auto j=0u; j<nBlocksY_; ++j) {
                std::stringstream hmSs;
                hmSs << heightMapFileNamePrefix_ << i+1 << '_' << j << ".png";
                img.loadFromFile(hmSs.str());
                col.emplace_back(img, 0.5f, 1.0f);
            }
            chunks_.push_back(std::move(col));
        }
        else {
            col.resize(nBlocksY_);
            chunks_.push_back(std::move(col));
        }

        //  compute
        for (auto j=0u; j<nBlocksY_; ++j) {
            //  bind the computation frame buffer
            glBindFramebuffer(GL_FRAMEBUFFER, framebufferId_);
            glViewport(0, 0, blockSizeX_, blockSizeY_);

            //  bind the inactive textures to framebuffer for writing
            for (auto k=0u; k<N_CHUNK_DATATEXTURES; ++k)
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+k, GL_TEXTURE_2D,
                                       chunks_[1][j].dataTexture_[k][!activeTextures_], 0);
            glDrawBuffers(sizeof(drawBuffers_), drawBuffers_);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                printf("Could not validate framebuffer\n");

            //  set up the computation shader
            computeShader_.use();
            for (auto k=0u; k<N_CHUNK_DATATEXTURES; ++k) {
                glActiveTexture(GL_TEXTURE0 + k);
                glBindTexture(GL_TEXTURE_2D, chunks_[1][j].dataTexture_[k][activeTextures_]);
                glUniform1i(glGetUniformLocation(computeShader_.getId(), "tex"), k);
            }

            //  draw the quad
            glBindVertexArray(vertexArrayId_);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            //  rebind the screen framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, 512, 512);
            visShader.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, chunks_[1][j].dataTexture_[0][activeTextures_]);
            glUniform1i(glGetUniformLocation(visShader.getId(), "tex"), 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(0);
        }
    }

    activeTextures_ = !activeTextures_;
}
