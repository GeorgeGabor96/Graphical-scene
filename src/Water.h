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

#define WATER_TALE_SIZE 100
#define WATER_MOVE_SPEED 0.09f // vezi ca probabil va trebuie micsorata aprox 0.03

namespace wat
{
    class Water
    {
    public:
        Water();
        Water(wat::WaterFrameBuffers waterFrameBuffers, const char * gugvMapPath, const char * normalMapPath);
        void setModel(glm::mat4 model);
        void setView(glm::mat4 view, glm::vec3 cameraPosition);
        void setProjection(glm::mat4 projection);
        void setShader(gps::Shader shader);
        void setLightPos(glm::vec3 lightPos);
        glm::vec3 getPosition();
        void draw(float frameTime, glm::vec3 lightColor, GLuint tex);
        void InitWater();
        void addPostion(glm::vec3 position);
        void setSkyColor(glm::vec3 skyColor);
    private:
        gps::Shader shader;
        std::vector<glm::vec3> positions;
        wat::WaterFrameBuffers waterFrameBuffers;
        GLuint waterVAO;
        GLuint waterVBO;
        GLuint modelLoc;
        GLuint viewLoc;
        GLuint projectionLoc;
        GLuint cameraPosLoc;
        GLuint gugvMap;
        GLuint normalMap;
        GLuint moveFactorLoc;
        GLuint lightColorLoc;
        GLuint lightPosLoc;
        GLuint skyColorLoc;
        float moveFactor;
        void InitLoc();
    };
}