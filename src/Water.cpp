#include "Water.h"


namespace wat
{
    Water::Water()
    {

    }

    void Water::addPostion(glm::vec3 position)
    {
        positions.push_back(position);
    }


    Water::Water(wat::WaterFrameBuffers waterFrameBuffers, const char * gugvMapPath, const char * normalMapPath)
    {
        this->shader = shader;
        this->waterFrameBuffers = waterFrameBuffers;
        this->gugvMap = gps::Model3D::ReadTextureFromFile(gugvMapPath);
        this->normalMap = gps::Model3D::ReadTextureFromFile(normalMapPath);
        this->moveFactor = 0.0f;
    }

    void Water::draw(float frameTime, glm::vec3 lightColor, GLuint tex)
    {
        shader.useShaderProgram();
        
        // enable alfa-blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUniform3fv(this->lightColorLoc, 1, glm::value_ptr(lightColor));

        glUniform1f(this->moveFactorLoc, moveFactor);

        this->moveFactor += frameTime * WATER_MOVE_SPEED;
        if (this->moveFactor > 1.0f)
        {
            moveFactor = 0.0f;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterFrameBuffers.getReflectionTexture());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, waterFrameBuffers.getRefractionTexture());

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, this->gugvMap);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, this->normalMap);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, waterFrameBuffers.getRefractionDepthTexture());

        glBindVertexArray(this->waterVAO);
        
        for (unsigned int i = 0; i < positions.size(); i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, positions[i]);
            model = glm::scale(model, glm::vec3(WATER_TALE_SIZE, WATER_TALE_SIZE, WATER_TALE_SIZE));
            glUniformMatrix4fv(this->modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        glBindVertexArray(0);

        // disable alfa-blending
        glDisable(GL_BLEND);
    }

    void Water::InitWater()
    {
        GLfloat waterPositions[] = 
        {
            -1.0f, 0.0f, -1.0f,
            -1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, -1.0f,
            1.0f, 0.0f, -1.0f,
            -1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &(this->waterVAO));
        glGenBuffers(1, &(this->waterVBO));
        
        glBindVertexArray(waterVAO);
        glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(waterPositions), &waterPositions, GL_STATIC_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

        glBindVertexArray(0);
    }


    void Water::setModel(glm::mat4 model)
    {
        shader.useShaderProgram(); 
        glUniformMatrix4fv(this->modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    }

    void setSkyColor(glm::vec3 skyColor)
    {

    }

    void Water::setView(glm::mat4 view, glm::vec3 cameraPosition)
    {
        shader.useShaderProgram(); 
        glUniformMatrix4fv(this->viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(this->cameraPosLoc, 1, glm::value_ptr(cameraPosition));
    }
    
    void Water::setProjection(glm::mat4 projection)
    {
        shader.useShaderProgram(); 
        glUniformMatrix4fv(this->projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }

    void Water::setSkyColor(glm::vec3(skyColor))
    {
        shader.useShaderProgram();
        glUniform3fv(this->skyColorLoc, 1, glm::value_ptr(skyColor));
    }

    void Water::setLightPos(glm::vec3 lightPos)
    {
        shader.useShaderProgram();
        glUniform3fv(this->lightPosLoc, 1, glm::value_ptr(lightPos));
    }

    void Water::setShader(gps::Shader shader)
    {
        this->shader = shader;
        InitWater();
        InitLoc();
    }

    void Water::InitLoc()
    {
        shader.useShaderProgram();

        this->modelLoc = glGetUniformLocation(shader.shaderProgram, "model");
        /*glm::mat4 model(1.0f);
        model = glm::translate(model, this->position);
        model = glm::scale(model, glm::vec3(WATER_TALE_SIZE, WATER_TALE_SIZE, WATER_TALE_SIZE));
        glUniformMatrix4fv(this->modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/

        this->viewLoc = glGetUniformLocation(shader.shaderProgram, "view");
        this->projectionLoc = glGetUniformLocation(shader.shaderProgram, "projection");
        this->cameraPosLoc = glGetUniformLocation(shader.shaderProgram, "cameraPos");
        
        GLuint reflectionTextureLoc = glGetUniformLocation(shader.shaderProgram, "reflectionTexture");
        glUniform1i(reflectionTextureLoc, 0);
        GLuint refractionTextureLoc = glGetUniformLocation(shader.shaderProgram, "refractionTexture");
        glUniform1i(refractionTextureLoc, 1);

        GLuint gugvMapLoc = glGetUniformLocation(shader.shaderProgram, "dudvMap");
        glUniform1i(gugvMapLoc, 2);

        GLuint normalMapLoc = glGetUniformLocation(shader.shaderProgram, "normalMap");
        glUniform1i(normalMapLoc, 3);

        GLuint depthMapLoc = glGetUniformLocation(shader.shaderProgram, "depthMap");
        glUniform1i(depthMapLoc, 4);

        this->moveFactorLoc = glGetUniformLocation(shader.shaderProgram, "moveFactor");
        this->cameraPosLoc = glGetUniformLocation(shader.shaderProgram, "cameraPos");
        this->lightColorLoc = glGetUniformLocation(shader.shaderProgram, "lightColor");
        this->lightPosLoc = glGetUniformLocation(shader.shaderProgram, "lightPos");
        this->skyColorLoc = glGetUniformLocation(shader.shaderProgram, "skyColor");
    }

    glm::vec3 Water::getPosition()
    {
        return glm::vec3(0);
    }
}