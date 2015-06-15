/**
    PerlinNoise.hpp

    @author Miika Lehtimäki
    @date   2015-06-09
**/

#ifndef EROSIONSIMULATOR_HPP
#define EROSIONSIMULATOR_HPP


#include <GL/glew.h>


struct Cell {
    unsigned baseLevel;     //  integer base level
    float groundLevel;      //  level inside cell (0-1)
    float groundHardness;   //  ground hardness

    float waterLevel;       //  water level (in addition to ground level)
    float waterSpeedX;
    float waterSpeedY;      //  water speed

    float waterDissolved;   //  amount of dissolved material in water
    float waterHardness;    //  and hardness of that material
};


class ErosionSimulator {
public:


private:

};


#endif // EROSIONSIMULATOR_HPP
