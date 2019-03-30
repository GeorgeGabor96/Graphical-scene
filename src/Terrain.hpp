#pragma once

#include <vector>
#include "Config.h"
#include "structs.hpp"
#include "Model3D.hpp"
#include "Entity.hpp"
#include "HeightsGenerator.hpp"

namespace enty
{
    class Terrain:public Entity
    {
    public:
        Terrain(glm::vec3 position);
        void generate(std::vector<str::TextureInfo> texturesInfo, const char * heightMapPath, bool procedural);
        float getTerrainHeight(float wordlX, float worldZ);

    private:
        float** heights;
        int heightsLength;
        float getHeight(unsigned char * image_data, int xMax, int yMax, int x, int y);
        glm::vec3 Terrain::getNormal(unsigned char * image_data, int xMax, int yMax,int x, int y);
        static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
        static float floatModulo(float a, float b);
        float getHeight(HeightsGenerator generator, int x, int y);
        glm::vec3 Terrain::getNormal(HeightsGenerator generator, int x, int y);
    };
}