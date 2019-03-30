#include "Particle.hpp"

namespace parti
{
    Particle::Particle(ParticleTexture texture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale)
    {
        this->texture = texture;
        this->position = position;
        this->velocity = velocity;
        this->gravityEffect = gravityEffect;
        this->lifeLength = lifeLength;
        this->rotation = rotation;
        this->scale = scale;
        this->elapsedTime = 0;
        this->texOffset1 = glm::vec2(0, 0);
        this->texOffset2 = glm::vec2(0, 0);
    }

    ParticleTexture Particle::getTexture()
    {
        return texture;
    }

    glm::vec3 Particle::getPosition()
    {
        return this->position;
    }

    float Particle::getRotation()
    {
        return this->rotation;
    }

    float Particle::getScale()
    {
        return this->scale;
    }

    bool Particle::update(glm::vec3 camPos, float frameTime)
    {
        glm::vec3 disV = camPos - position;
        distanceCamera = disV.x * disV.x + disV.y * disV.y; // lenght square
        velocity.y += GRAVITY * gravityEffect * frameTime;
        //glm::vec3 posChange = glm::vec3(velocity);
        //posChange *= frameTime;
        position += velocity * frameTime;
        updateTextureCoordInfo();
        elapsedTime += frameTime;
        return elapsedTime < lifeLength ? true: false;
    }

    glm::vec2 Particle::getTexOffset1()
    {
        return texOffset1;
    }

    glm::vec2 Particle::getTexOffset2()
    {
        return texOffset2;
    }
    
    float Particle::getBlend()
    {
        return blend;
    }

    float Particle::getDistanceCamera()
    {
        return distanceCamera;
    }

    void Particle::updateTextureCoordInfo()
    {
        float lifeFactor = elapsedTime / lifeLength;
        int stageCount = texture.getNumberOfRows() * texture.getNumberOfRows();
        float atlasProgression = lifeFactor * stageCount;
        int index1 = (int)floor(atlasProgression);
        int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
        blend = atlasProgression - index1;
        setTextureOffset(texOffset1, index1);
        setTextureOffset(texOffset2, index2);
    }

    void Particle::setTextureOffset(glm::vec2 offset, int index)
    {
        int nrRows = texture.getNumberOfRows();
        int column = index % nrRows;
        int row = index / nrRows;
        offset.x = (float)column / nrRows;
        offset.y = (float)row / nrRows;
        //printf("%f %f\n", offset.x, offset.y);
    }

}