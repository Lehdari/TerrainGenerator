/**
    ErosionSimulator.hpp

    @version    0.1
    @author     Miika 'LehdaRi' Lehtimäki
    @date       2015-06-18

**/


#ifndef EROSIONSIMULATOR_HPP
#define EROSIONSIMULATOR_HPP


#include "Shader.hpp"

#include <deque>
#include <vector>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>


#define N_CHUNK_DATATEXTURES 1


/*struct Cell {
    unsigned baseLevel;     //  integer base level
    float groundLevel;      //  level inside cell (0-1)
    float groundHardness;   //  ground hardness

    float waterLevel;       //  water level (in addition to ground level)
    float waterSpeedX;
    float waterSpeedY;      //  water speed

    float waterDissolved;   //  amount of dissolved material in water
    float waterHardness;    //  and hardness of that material
};*/


class Chunk {
public:
    friend class ErosionSimulator;

    Chunk(void);
    /*
        heightMap:  sfml image of the heightmap
        hmMin/hmMax: value range to map the heightmap to
    */
    Chunk(const sf::Image& heightMap, float hmMin, float hmMax);
    ~Chunk(void);

    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;
    Chunk(Chunk&&);
    Chunk& operator=(Chunk&&);

private:
    GLuint dataTexture_[N_CHUNK_DATATEXTURES][2];
};


class ErosionSimulator {
public:
    ErosionSimulator(Shader& computeShader,
                     unsigned blockSizeX, unsigned blockSizeY,
                     unsigned nBlocksX, unsigned nBlocksY,
                     const std::string& heightMapFileNamePrefix);
    ~ErosionSimulator(void);

    ErosionSimulator(const ErosionSimulator&) = delete;
    ErosionSimulator(ErosionSimulator&&) = delete;
    ErosionSimulator& operator=(const ErosionSimulator&) = delete;
    ErosionSimulator& operator=(ErosionSimulator&&) = delete;

    void erode(Shader& visShader);

private:
    Shader& computeShader_;
    unsigned blockSizeX_, blockSizeY_;
    unsigned nBlocksX_, nBlocksY_;
    std::string heightMapFileNamePrefix_;

    GLuint vertexArrayId_;
    GLuint vertexBufferId_;
    GLuint framebufferId_;
    GLenum drawBuffers_[N_CHUNK_DATATEXTURES];
    int activeTextures_;

    std::deque<std::vector<Chunk>> chunks_;

    static float quad__[18];
};


#endif // EROSIONSIMULATOR_HPP
