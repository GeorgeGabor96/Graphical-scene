#include "HeightsGenerator.hpp"


namespace enty
{
    HeightsGenerator::HeightsGenerator()
    {
        std::uniform_real_distribution<float> distribution(1, 6);
        generator.seed(time(NULL));
        seed = distribution(generator);
    }

    float HeightsGenerator::generateHeight(int x, int z)
    {
        float total = getInterpolatedNoise(x/8.0f, z/8.0f) * AMPLITUDE;
        return total;
    }

    float HeightsGenerator::getSmoothNoise(int x, int z) {
        float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
            + getNoise(x + 1, z + 1)) / 16.0f;
        float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
            + getNoise(x, z + 1)) / 8.0f;
        float center = getNoise(x, z) / 4.0f;
        return corners + sides + center;
    }

    float HeightsGenerator::getNoise(int x, int z)
    {
        std::uniform_real_distribution<float> distribution(1, 1000000000);
        generator.seed(seed + x * 49632 + z * 325176);

        float c = (float)(distribution(generator) / (1000000000)) * 2.0f - 1.0f;

        return c;
    }

    float HeightsGenerator::getInterpolatedNoise(float x, float z) {
        int intX = (int)x;
        int intZ = (int)z;
        float fracX = x - intX;
        float fracZ = z - intZ;

        float v1 = getSmoothNoise(intX, intZ);
        float v2 = getSmoothNoise(intX + 1, intZ);
        float v3 = getSmoothNoise(intX, intZ + 1);
        float v4 = getSmoothNoise(intX + 1, intZ + 1);
        float i1 = interpolate(v1, v2, fracX);
        float i2 = interpolate(v3, v4, fracX);
        return interpolate(i1, i2, fracZ);
    }

    float HeightsGenerator::interpolate(float a, float b, float blend) {
        double theta = blend * M_PI;
        float f = (float)(1.0f - cos(theta)) * 0.5f;
        return a * (1.0f - f) + b * f;
    }

}