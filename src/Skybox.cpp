//
//  SkyBox.cpp
//  Lab10(CubeMaps)
//
//  Created by CGIS on 16/12/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#include "SkyBox.hpp"

namespace sky {
    
    SkyBox::SkyBox()
    {
        this->rotation = 0.0f;
        this->blendFactor = 0.0f;
        this->time = 0.0f;
    }
    
    void SkyBox::init(gps::Shader shader, glm::vec3 fogColor)
    {
        shader.useShaderProgram();
    
        GLuint vectorLocation = glGetUniformLocation(shader.shaderProgram, "fogColor");
        glUniform3fv(vectorLocation, 1, glm::value_ptr(fogColor));
        this->blendFactorLocation = glGetUniformLocation(shader.shaderProgram, "blendFactor");
    }


    void SkyBox::Load(std::vector<const GLchar*> cubeMapFacesDay, std::vector<const GLchar*> cubeMapFacesNight)
    {
        cubemapTextureDay = LoadSkyBoxTextures(cubeMapFacesDay, GL_TEXTURE0);
        cubemapTextureNight = LoadSkyBoxTextures(cubeMapFacesNight, GL_TEXTURE1);
        InitSkyBox();
    }
    
    void SkyBox::update(gps::Shader shader, float frameTime)
    {
        this->rotation += frameTime * SKY_ROTATION_SPEED;
        if (this->rotation > 360.0f)
        {
            this->rotation = 0.0f;
        }

        float lightIncrement = frameTime * SKY_LIGHT_FACTOR;

        time += frameTime * SKY_DAY_NIGHT_SPEED;
        if (time > 24000.0f)
        {
            time -= 24000.0f;
        }
        GLuint texture1;
        GLuint texture2;

        if (time >= 0.0f && time < 5000.0f) {
            texture1 = cubemapTextureNight;
            texture2 = cubemapTextureNight;
            blendFactor = (time - 0.0f) / (5000.0f - 0.0f);
        }
        else if (time >= 5000.0f && time < 8000.0f) {
            texture1 = cubemapTextureNight;
            texture2 = cubemapTextureDay;
            blendFactor = (time - 5000.0f) / (8000.0f - 5000.0f);
            if (skyLight.x < SKY_DAY_COLOR)
            {
                skyLight.x += lightIncrement;
            }
        }
        else if (time >= 8000.0f && time < 21000.0f) {
            texture1 = cubemapTextureDay;
            texture2 = cubemapTextureDay;
            blendFactor = (time - 8000.0f) / (21000.0f - 8000.0f);
        }
        else {
            texture1 = cubemapTextureDay;
            texture2 = cubemapTextureNight;
            blendFactor = (time - 21000.0f) / (24000 - 21000.0f);
            if (skyLight.x > SKY_NIGHT_COLOR)
            {
                skyLight.x -= lightIncrement;
            }
        }
        skyLight.y = skyLight.x;
        skyLight.z = skyLight.x;

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader.shaderProgram, "skyboxDay"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader.shaderProgram, "skyboxNight"), 1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
        glUniform1f(this->blendFactorLocation, blendFactor);
    }

    void SkyBox::Draw(gps::Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float frameTime)
    {
        shader.useShaderProgram();
        this->update(shader, frameTime);

        //set the view and projection matrices
        glm::mat4 transformedView = glm::mat4(glm::mat3(viewMatrix));
        transformedView = glm::rotate(transformedView, glm::radians(this->rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(transformedView));
        glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        
        glDepthFunc(GL_LEQUAL);
        
        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        glDepthFunc(GL_LESS);
    }
    
    GLuint SkyBox::LoadSkyBoxTextures(std::vector<const GLchar*> skyBoxFaces, unsigned int activeTexture)
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glActiveTexture(activeTexture);
        
        int width,height, n;
        unsigned char* image;
        int force_channels = 3;
        
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for(GLuint i = 0; i < skyBoxFaces.size(); i++)
        {
            image = stbi_load(skyBoxFaces[i], &width, &height, &n, force_channels);
            if (!image) {
                fprintf(stderr, "ERROR: could not load %s\n", skyBoxFaces[i]);
                return false;
            }
            glTexImage2D(
                         GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
                         );
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        
        return textureID;
    }
    
    void SkyBox::InitSkyBox()
    {
        GLfloat skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            
            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };
        
        glGenVertexArrays(1, &(this->skyboxVAO));
        glGenBuffers(1, &skyboxVBO);
        
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        
        glBindVertexArray(0);
    }

    glm::vec3 SkyBox::getSkyLight()
    {
        return skyLight;
    }

}
