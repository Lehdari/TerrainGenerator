#include "Perlin3D.hpp"


Perlin3D::Perlin3D(unsigned xSize, unsigned ySize, unsigned zSize, unsigned depth,
                   std::default_random_engine& rnd, float amplitude,
                   Perlin3D* left, Perlin3D* above) :
    xSize_(xSize),
    ySize_(ySize),
    zSize_(zSize),
    left_(left),
    right_(nullptr),
    above_(above),
    below_(nullptr)
{
    if (depth > 0) {
        next_ = std::unique_ptr<Perlin3D>(new Perlin3D(xSize*2, ySize*2, zSize*2, depth-1,
                                                       rnd, amplitude*0.5f,
                                                       left ? left->getNext() : nullptr,
                                                       above ? above->getNext() : nullptr));
    }
    else
        next_ = std::unique_ptr<Perlin3D>(nullptr);

    data_.reserve(xSize);

    for (auto i=0u; i<xSize; ++i) {
        std::vector<std::vector<float>> layer;
        layer.reserve(ySize);
        for (auto j=0u; j<ySize; ++j) {
            std::vector<float> row;
            row.reserve(zSize);
            for (auto k=0u; k<zSize; ++k) {
                if (left_ && i == 0)
                    row.push_back(left_->getDataPoint(xSize-1, j, k));
                else if (above_ && j == 0)
                    row.push_back(above_->getDataPoint(i, ySize-1, k));
                else
                    row.push_back(((rnd()-rnd.min())/(double)(rnd.max()-rnd.min()))*amplitude - amplitude*0.5f);
            }
            layer.push_back(row);
        }
        data_.push_back(std::move(layer));
    }
}

void Perlin3D::setLeft(Perlin3D* p) {
    left_ = p;
}

void Perlin3D::setRight(Perlin3D* p) {
    right_ = p;
}

void Perlin3D::setAbove(Perlin3D* p) {
    above_ = p;
}

void Perlin3D::setBelow(Perlin3D* p) {
    below_ = p;
}

Perlin3D* Perlin3D::getNext(void) const {
    return next_.get();
}

float Perlin3D::getDataPoint(unsigned x, unsigned y, unsigned z) const {
    return data_[x][y][z];
}

float Perlin3D::sample(float edgeSize, float x, float y, float z) const {
    float p = ((xSize_-1)/edgeSize)*x;
    unsigned id = (unsigned)p;
    float ip = p-id;
    int sId[4] = { (int)id-1, (int)id, (int)id+1, (int)id+2 };

    float s[4];

    if (sId[0] < 0) {
        if (left_)
            s[0] = left_->sampleLayer(edgeSize, xSize_-2, y, z);
        else
            s[0] = sampleLayer(edgeSize, sId[1], y, z);
    }
    else
        s[0] = sampleLayer(edgeSize, sId[0], y, z);

    if (sId[3] >= (int)xSize_) {
        if (right_)
            s[3] = right_->sampleLayer(edgeSize, 1, y, z);
        else
            s[3] = sampleLayer(edgeSize, sId[2], y, z);
    }
    else
        s[3] = sampleLayer(edgeSize, sId[3], y, z);

    s[1] = sampleLayer(edgeSize, sId[1], y, z);
    s[2] = sampleLayer(edgeSize, sId[2], y, z);

    return s[1] + 0.5 * ip*(s[2] - s[0] + ip*(2.0*s[0] - 5.0*s[1] + 4.0*s[2] - s[3] + ip*(3.0*(s[1] - s[2]) + s[3] - s[0])))
           + (next_ ? next_->sample(edgeSize, x, y, z) : 0.0f);
}

float Perlin3D::sampleLayer(float edgeSize, unsigned x, float y, float z) const {
    float p = ((ySize_-1)/edgeSize)*y;
    unsigned id = (unsigned)p;
    float ip = p-id;
    int sId[4] = { (int)id-1, (int)id, (int)id+1, (int)id+2 };

    float s[4];

    if (sId[0] < 0) {
        if (above_)
            s[0] = above_->sampleRow(edgeSize, x, ySize_-2, z);
        else
            s[0] = sampleRow(edgeSize, x, sId[1], z);
    }
    else
        s[0] = sampleRow(edgeSize, x, sId[0], z);

    if (sId[3] >= (int)ySize_) {
        if (below_)
            s[3] = below_->sampleRow(edgeSize, x, 1, z);
        else
            s[3] = sampleRow(edgeSize, x, sId[2], z);
    }
    else
        s[3] = sampleRow(edgeSize, x, sId[3], z);

    s[1] = sampleRow(edgeSize, x, sId[1], z);
    s[2] = sampleRow(edgeSize, x, sId[2], z);

    return s[1] + 0.5 * ip*(s[2] - s[0] + ip*(2.0*s[0] - 5.0*s[1] + 4.0*s[2] - s[3] + ip*(3.0*(s[1] - s[2]) + s[3] - s[0])));
}

float Perlin3D::sampleRow(float edgeSize, unsigned x, unsigned y, float z) const {
    float p = ((zSize_-1)/edgeSize)*z;
    unsigned id = (unsigned)p;
    float ip = p-id;
    int sId[4] = { std::max(0, (int)id-1), (int)id, std::min((int)zSize_-1, (int)id+1), std::min((int)zSize_-1, (int)id+2) };

    float s[4] = { data_[x][y][sId[0]],
                   data_[x][y][sId[1]],
                   data_[x][y][sId[2]],
                   data_[x][y][sId[3]] };

    return s[1] + 0.5 * ip*(s[2] - s[0] + ip*(2.0*s[0] - 5.0*s[1] + 4.0*s[2] - s[3] + ip*(3.0*(s[1] - s[2]) + s[3] - s[0])));
}
