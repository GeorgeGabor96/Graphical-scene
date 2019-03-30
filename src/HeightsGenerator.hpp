#pragma once

#include <vector>
#include "Config.h"
#include "structs.hpp"
#include "Model3D.hpp"
#include "Entity.hpp"
#include <time.h>
#include <random>

#define AMPLITUDE 70.0f

namespace enty
{
    class HeightsGenerator
    {
    public:
        HeightsGenerator();
        float generateHeight(int x, int z);
        float getSmoothNoise(int x, int z);
        float getInterpolatedNoise(float x, float z);
        float interpolate(float a, float b, float blend);
    private:
        float seed;
        float r;
        std::default_random_engine generator;
        float getNoise(int x, int z); // returneaza numar intre [-1.1]
    };
}