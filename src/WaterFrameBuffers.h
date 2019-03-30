#pragma once

#include "Shader.hpp"
#include <vector>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Config.h"
#include "glm/gtc/matrix_transform.hpp"

#define REFLECTION_WIDTH 320
#define REFLECTION_HEIGHT 180

#define REFRACTION_WIDTH 1280
#define REFRACTION_HEIGHT 720

namespace wat
{
    class WaterFrameBuffers
    {
    public:
        WaterFrameBuffers();
        void unbindCurrentFrameBuffer();
        void bindReflectionFrameBuffer();
        void bindRefractionFrameBuffer();
        GLuint getReflectionTexture();
        GLuint getRefractionTexture();
        GLuint getRefractionDepthTexture();
        void init();
    private:
        GLuint reflectionFrameBuffer;
        GLuint reflectionTexture;
        GLuint reflectionDepthBuffer;

        GLuint refractionFrameBuffer;
        GLuint refractionTexture;
        GLuint refractionDepthTexture;
        
        void initialiseReflectionFrameBuffer();
        void initialiseRefractionFrameBuffer();
        GLuint createFrameBuffer(); 
        GLuint createTextureAttachment(int width, int height);
        GLuint createDepthBufferAttachment(int width, int height); 
        GLuint createDepthTextureAttachment(int width, int height);
        void bindFrameBuffer(GLuint frameBuffer, int width, int height);
    };
}