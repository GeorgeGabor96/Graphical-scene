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

namespace parti
{
    class ParticleTexture
    {
    public:
        ParticleTexture();
        ParticleTexture(const char * texturePath, int numberOfRows);
        GLuint getTextureID();
        int getNumberOfRows();
    private:
        GLuint textureID;
        int numberOfRows;
    };
}