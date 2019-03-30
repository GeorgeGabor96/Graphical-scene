#include "WaterFrameBuffers.h"

namespace wat
{
    WaterFrameBuffers::WaterFrameBuffers()
    {
        
    }

    void WaterFrameBuffers::init()
    {
        initialiseReflectionFrameBuffer();
        initialiseRefractionFrameBuffer();
    }

    void WaterFrameBuffers::bindReflectionFrameBuffer()
    {
        bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
    }

    void WaterFrameBuffers::bindRefractionFrameBuffer()
    {
        bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
    }
    
    GLuint WaterFrameBuffers::getReflectionTexture()
    {
        return reflectionTexture;
    }

    GLuint WaterFrameBuffers::getRefractionTexture()
    {
        return refractionTexture;
    }

    GLuint WaterFrameBuffers::getRefractionDepthTexture()
    {
        return refractionDepthTexture;
    }

    void WaterFrameBuffers::initialiseReflectionFrameBuffer()
    {
        this->reflectionFrameBuffer = createFrameBuffer();
        this->reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
        this->reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
        unbindCurrentFrameBuffer();
    }

    void WaterFrameBuffers::initialiseRefractionFrameBuffer()
    {
        this->refractionFrameBuffer = createFrameBuffer();
        this->refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
        this->refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
        unbindCurrentFrameBuffer();
    }

    void WaterFrameBuffers::unbindCurrentFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }


    void WaterFrameBuffers::bindFrameBuffer(GLuint frameBuffer, int width, int height)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glViewport(0, 0, width, height);
    }


    GLuint WaterFrameBuffers::createFrameBuffer()
    {
        GLuint frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        return frameBuffer;
    }

    GLuint WaterFrameBuffers::createTextureAttachment(int width, int height)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            NULL
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // pune sa se randeze pe textura asta
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
        
        return texture;
    }

    GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT32,
            width,
            height,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

        return texture;
    }

    GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height)
    {
        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

        return depthBuffer;
    }
}