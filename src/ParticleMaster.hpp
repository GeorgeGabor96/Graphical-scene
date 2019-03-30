#pragma once

#include <stdio.h>
#include "Shader.hpp"
#include <vector>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Config.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Model3D.hpp"
#include "Particle.hpp"
#include "ParticleRenderer.hpp"
#include "ParticleTexture.hpp"

namespace parti
{
    class ParticleMaster
    {
    public:
        ParticleMaster();
        ParticleMaster(ParticleTexture texture, gps::Shader shader, glm::mat4 projectionMatrix, float pps, float speed, float gravityComplient, float lifeLength, float scale);
        void update(glm::vec3 camPos, float frameTime);
        void renderParticles(glm::mat4 viewMatrix);
        void addParticle(glm::vec3 position);
        void addParticle();
        void add(glm::vec3 position, glm::vec3 velocity);
        void count();

        void setDirection(glm::vec3 direction, float deviation);
        void randomizeRotation();
        void setSpeedError(float error);
        void setLifeError(float error);
        void setScaleError(float error);
        void generateParticles(float frameTime, glm::vec3 systemCenter);

    private:
        std::vector<parti::Particle*> particles;
        parti::ParticleRenderer renderer;
        float pps;
        float averageSpeed;
        float gravityComplient;
        float averageLifeLength;
        float averageScale;
        float speedError;
        float lifeError;
        float scaleError;
        bool randomRotation;
        glm::vec3 direction;
        float directionDeviation;
        ParticleTexture texture;


        void emitParticle(glm::vec3 center);
        void insertionSort();
        float generateValue(float average, float errorMargin);
        float generateRotation();
        glm::vec3 generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle);
        glm::vec3 generateRandomUnitVector();

    };
}