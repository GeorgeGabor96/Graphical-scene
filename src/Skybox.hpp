//
//  SkyBox.hpp
//  Lab10(CubeMaps)
//
//  Created by CGIS on 16/12/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#ifndef SkyBox_hpp
#define SkyBox_hpp

#include <stdio.h>
#include "Shader.hpp"
#include <vector>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Config.h"
#include "glm/gtc/matrix_transform.hpp"

namespace sky {
    class SkyBox
    {
    public:
        SkyBox();
        void Load(std::vector<const GLchar*> cubeMapFacesDay, std::vector<const GLchar*> cubeMapFacesNight);
        void Draw(gps::Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float frameTime);
        void init(gps::Shader shader, glm::vec3 fogColor);
        glm::vec3 getSkyLight();

    private:
        float rotation;
        float blendFactor;
        float blendFactorLocation;
        float time;
        glm::vec3 skyLight;
        GLuint skyboxVAO;
        GLuint skyboxVBO;
        GLuint cubemapTextureDay;
        GLuint cubemapTextureNight;
        GLuint LoadSkyBoxTextures(std::vector<const GLchar*> cubeMapFaces, unsigned int activeTexture);
        void InitSkyBox();
        void update(gps::Shader shader, float frameTime);
    };
}

#endif /* SkyBox_hpp */
