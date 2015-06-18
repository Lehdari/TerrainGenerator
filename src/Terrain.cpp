#include "Terrain.hpp"
#include "Perlin3D.hpp"
#include "Array3D.hpp"

#include <sstream>
#include <memory>
#include <SFML/Graphics.hpp>


void generatePerlin(void) {
    const unsigned edgeSize = 256;
    const unsigned nChunksX = 16;
    const unsigned nChunksY = 16;

    const unsigned perlinInitSize = 4;
    const unsigned perlinDepth = 5;

    std::default_random_engine rnd(0xBADA55);

    printf("allocating array..\n");
    Array3D<float, edgeSize, edgeSize, edgeSize> arr;

    printf("generating initial perlin noise data..\n");
    std::unique_ptr<Perlin3D> perlin[3][nChunksY];

    for (auto i=0u; i<nChunksY; ++i) {
        perlin[0][i] = std::unique_ptr<Perlin3D>(nullptr);
        if (i>0) {
            perlin[1][i] = std::unique_ptr<Perlin3D>(new Perlin3D(perlinInitSize, perlinInitSize, perlinInitSize, perlinDepth, rnd, 1.0f, nullptr, perlin[1][i-1].get()));
            perlin[2][i] = std::unique_ptr<Perlin3D>(new Perlin3D(perlinInitSize, perlinInitSize, perlinInitSize, perlinDepth, rnd, 1.0f, perlin[1][i].get(), perlin[2][i-1].get()));
            perlin[1][i-1]->setBelow(perlin[1][i].get());
            perlin[2][i-1]->setBelow(perlin[2][i].get());
        }
        else {
            perlin[1][i] = std::unique_ptr<Perlin3D>(new Perlin3D(perlinInitSize, perlinInitSize, perlinInitSize, perlinDepth, rnd, 1.0f, nullptr, nullptr));
            perlin[2][i] = std::unique_ptr<Perlin3D>(new Perlin3D(perlinInitSize, perlinInitSize, perlinInitSize, perlinDepth, rnd, 1.0f, perlin[1][i].get(), nullptr));
        }
        perlin[1][i]->setRight(perlin[2][i].get());
    }

    for (auto cx=0u; cx<nChunksX; ++cx) {
        for (auto cy=0u; cy<nChunksY; ++cy) {
            printf("creating chunk %u/%u..\n", cx*nChunksY+cy+1, nChunksX*nChunksY);
            for (auto i=0u; i<edgeSize; ++i) {
                for (auto j=0u; j<edgeSize; ++j) {
                    for (auto k=0u; k<edgeSize; ++k) {
                        arr(i, j, k) = perlin[1][cy]->sample(edgeSize, i+0.5f, j+0.5f, k+0.5f);
                    }
                }
                printf("sampling.. %0.2f%%\r", (i*100.0f)/(edgeSize));
            }
            printf("\n");

            printf("saving to file..\n");
            std::stringstream ss;
            ss << "res/perlin_" << cx << "_" << cy << ".bin";
            arr.saveToFile(ss.str());

            printf("saving top layer to image..\n");
            sf::Image img;
            img.create(edgeSize, edgeSize);
            for (auto i=0u; i<edgeSize; ++i) {
                for (auto j=0u; j<edgeSize; ++j) {
                    float c = arr(i, j, 0)*128 + 128;
                    if (c > 255.0f) c = 255.0f;
                    img.setPixel(i, j, sf::Color(c, c, c));
                }
            }

            std::stringstream imgSs;
            imgSs << "res/terrain" << cx << "_" << cy << ".png";
            img.saveToFile(imgSs.str());
        }

        printf("generating new perlin noise data..\n");
        for (auto i=0u; i<nChunksY; ++i) {
            perlin[0][i] = std::move(perlin[1][i]);
            perlin[1][i] = std::move(perlin[2][i]);
            if (i>0) {
                perlin[2][i] = std::unique_ptr<Perlin3D>(new Perlin3D(perlinInitSize, perlinInitSize, perlinInitSize, perlinDepth, rnd, 1.0f, perlin[1][i].get(), perlin[2][i-1].get()));
                perlin[2][i-1]->setBelow(perlin[2][i].get());
            }
            else {
                perlin[2][i] = std::unique_ptr<Perlin3D>(new Perlin3D(perlinInitSize, perlinInitSize, perlinInitSize, perlinDepth, rnd, 1.0f, perlin[1][i].get(), nullptr));
            }
            perlin[1][i]->setRight(perlin[2][i].get());
        }
    }
}
