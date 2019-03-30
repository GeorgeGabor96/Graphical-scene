#pragma once

#include "glm\vec3.hpp"

namespace light
{
    class LightSource
    {
    public:
        LightSource(glm::vec3 position, glm::vec3 color);
        LightSource(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation);

        glm::vec3 getPosition();
        glm::vec3 getColor();
        glm::vec3 getAttenuation();

    private:
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 attenuation;
    };
}
