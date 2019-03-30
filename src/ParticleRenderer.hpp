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
#include "ParticleTexture.hpp"

#define VERTICES_COUNT 8

namespace parti
{
    class ParticleRenderer
    {
    public:
        ParticleRenderer();
        ParticleRenderer(gps::Shader shader, glm::mat4 projectionMatrix);
        void draw(ParticleTexture texture, std::vector<parti::Particle*> particles, glm::mat4 viewMatrix);
        void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix);

    private:
        gps::Shader shader;
        GLuint VAO;
        GLuint VBO;
        GLuint modelViewMatrixLoc;
        GLuint texOffset1Loc;
        GLuint texOffset2Loc;
        GLuint texCoordInfoLoc;

        void initRenderer();
        void initLoc(glm::mat4 projectionMatrix);

    };
}