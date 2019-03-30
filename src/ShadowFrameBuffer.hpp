#pragma once

#include "Shader.hpp"
#include <vector>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Config.h"
#include "glm/gtc/matrix_transform.hpp"

namespace shadow
{
    class ShadowFrameBuffer
    {
    public:
        ShadowFrameBuffer(int width, int height);
        void bindFrameBuffer();
        void unbindFrameBuffer();
        GLuint getShadowMap();
        void initFrameBuffer();

    private:
        int width;
        int height;
        GLuint FBO;
        GLuint shadowMap;
    
        void bindFrameBuffer(GLuint frameBuffer, int width, int height);
        GLuint createFrameBuffer();
        GLuint createDepthBufferAttachment(int width, int height);
    };
}