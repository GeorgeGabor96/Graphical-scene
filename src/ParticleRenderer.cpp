#include "ParticleRenderer.hpp"

namespace parti
{

    ParticleRenderer::ParticleRenderer() {};

    ParticleRenderer::ParticleRenderer(gps::Shader shader, glm::mat4 projectionMatrix)
    {
        this->shader = shader;
        initRenderer();
        initLoc(projectionMatrix);
    }

    void ParticleRenderer::initRenderer()
    {
        const GLfloat vertices[VERTICES_COUNT] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };

        glGenVertexArrays(1, &(this->VAO));
        glGenBuffers(1, &(this->VBO));

        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);

        glBindVertexArray(0);
    }

    void ParticleRenderer::initLoc(glm::mat4 projectionMatrix)
    {
        shader.useShaderProgram();
        GLuint projectionMatrixLoc = glGetUniformLocation(shader.shaderProgram, "projectionMatrix");
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
        modelViewMatrixLoc = glGetUniformLocation(shader.shaderProgram, "modelViewMatrix");
        GLuint particleTextureLoc = glGetUniformLocation(shader.shaderProgram, "particleTexture");
        glUniform1i(particleTextureLoc, 0);

        texOffset1Loc = glGetUniformLocation(shader.shaderProgram, "texOffset1");
        texOffset2Loc = glGetUniformLocation(shader.shaderProgram, "texOffset2");
        texCoordInfoLoc = glGetUniformLocation(shader.shaderProgram, "texCoordInfo");
    }

    void ParticleRenderer::draw(ParticleTexture texture, std::vector<parti::Particle*> particles, glm::mat4 viewMatrix)
    {
        shader.useShaderProgram();
        glBindVertexArray(VAO);
        glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE);//bun la foc
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(false); // nu randa in depth buffer
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getTextureID());

        for (int i = 0; i < particles.size(); i++)
        {
            parti::Particle parti = (*particles[i]);
            glUniform2fv(texOffset1Loc, 1, glm::value_ptr(parti.getTexOffset1()));
            glUniform2fv(texOffset2Loc, 1, glm::value_ptr(parti.getTexOffset2()));
            glUniform2fv(texCoordInfoLoc, 1, glm::value_ptr(glm::vec2(texture.getNumberOfRows(), parti.getBlend())));

            updateModelViewMatrix(parti.getPosition(), parti.getRotation(), parti.getScale(), viewMatrix);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, VERTICES_COUNT);
           // printf("%f %f\n", parti.getPosition().x, parti.getPosition().y);
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        glDepthMask(true);
        glDisable(GL_BLEND);
    }

    // asigura ca modelView matrix nu are rotatie pentru ca particulele sa fie cu fata la camera
    void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix)
    {
        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        // coloanele pe primul index, liniile pe al doilea
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
        //modelMatrix[0, 0] = viewMatrix[0, 0];
        //modelMatrix[1, 0] = viewMatrix[0, 1];
        //modelMatrix[2, 0] = viewMatrix[0, 2];
        //modelMatrix[0, 1] = viewMatrix[1, 0];
        //modelMatrix[1, 1] = viewMatrix[1, 1];
        //modelMatrix[2, 1] = viewMatrix[1, 2];
        //modelMatrix[0, 2] = viewMatrix[2, 0];
        //modelMatrix[1, 2] = viewMatrix[2, 1];
        //modelMatrix[2, 2] = viewMatrix[2, 2];
        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

        glUniformMatrix4fv(modelViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    }


}