/**
    PerlinNoise.hpp

    @author Miika Lehtimäki
    @date   2015-06-09
**/

#ifndef PERLIN3D_HPP
#define PERLIN3D_HPP


#include <vector>
#include <memory>
#include <random>


class Perlin3D {
public:
    Perlin3D(unsigned xSize, unsigned ySize, unsigned zSize, unsigned depth,
             std::default_random_engine& rnd, float amplitude,
             Perlin3D* left = nullptr, Perlin3D* above = nullptr);

    void setLeft(Perlin3D* p);
    void setRight(Perlin3D* p);
    void setAbove(Perlin3D* p);
    void setBelow(Perlin3D* p);

    Perlin3D* getNext(void) const;

    float getDataPoint(unsigned x, unsigned y, unsigned z) const;
    float sample(float edgeSize, float x, float y, float z) const;

private:
    unsigned xSize_;
    unsigned ySize_;
    unsigned zSize_;

    std::vector<std::vector<std::vector<float>>> data_;
    std::unique_ptr<Perlin3D> next_;

    Perlin3D* left_;
    Perlin3D* right_;
    Perlin3D* above_;
    Perlin3D* below_;

    float sampleLayer(float edgeSize, unsigned x, float y, float z) const;
    float sampleRow(float edgeSize, unsigned x, unsigned y, float z) const;
};


#endif // PERLIN3D_HPP
