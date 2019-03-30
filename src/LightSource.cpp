#include "LightSource.hpp"


namespace light
{
    LightSource::LightSource(glm::vec3 position, glm::vec3 color)
    {
        this->position = position;
        this->color = color;
        this->attenuation = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    LightSource::LightSource(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation)
    {
        this->position = position;
        this->color = color;
        this->attenuation = attenuation;
    }

    glm::vec3 LightSource::getPosition()
    {
        return this->position;
    }

    glm::vec3 LightSource::getColor()
    {
        return this->color;
    }

    glm::vec3 LightSource::getAttenuation()
    {
        return this->attenuation;
    }
}