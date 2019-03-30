#include "ShadowFrameBuffer.hpp"

namespace shadow
{
    ShadowFrameBuffer::ShadowFrameBuffer(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    void ShadowFrameBuffer::bindFrameBuffer()
    {
        bindFrameBuffer(FBO, width, height);
    }

    void ShadowFrameBuffer::unbindFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    GLuint ShadowFrameBuffer::getShadowMap()
    {
        return shadowMap;
    }

    void ShadowFrameBuffer::initFrameBuffer()
    {
        FBO = createFrameBuffer();
        shadowMap = createDepthBufferAttachment(width, height);
        unbindFrameBuffer();
    }

    void ShadowFrameBuffer::bindFrameBuffer(GLuint frameBuffer, int width, int height)
    {
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
        glViewport(0, 0, width, height);
    }

    GLuint ShadowFrameBuffer::createFrameBuffer()
    {
        GLuint frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
        //glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        return frameBuffer;
    }

    GLuint ShadowFrameBuffer::createDepthBufferAttachment(int width, int height)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT,
            width,
            height,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

        return texture;
    }
}