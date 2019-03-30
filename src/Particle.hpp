#pragma once

#include <stdio.h>
#include "Shader.hpp"
#include <vector>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Config.h"
#include "glm/gtc/matrix_transform.hpp"
#include "WaterFrameBuffers.h"
#include "Model3D.hpp"
#include "ParticleTexture.hpp"

namespace parti
{
    class Particle
    {
    public:
        Particle(ParticleTexture texture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale);
        glm::vec3 getPosition();
        float getRotation();
        float getScale();
        ParticleTexture getTexture();
        bool update(glm::vec3 camPos, float frameTime); // false daca particula e moarta
        glm::vec2 getTexOffset1();
        glm::vec2 getTexOffset2();
        float getBlend();
        float getDistanceCamera();

    private:
        glm::vec3 position;
        glm::vec3 velocity;
        float gravityEffect;
        float lifeLength;
        float rotation;
        float scale;
        float elapsedTime;
        ParticleTexture texture;
        glm::vec2 texOffset1;
        glm::vec2 texOffset2;
        float blend;
        float distanceCamera;

        void updateTextureCoordInfo();
        void setTextureOffset(glm::vec2 offset, int index);

    };
}