#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "GLEW/glew.h"

namespace gps
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture
    {
        GLuint id;
        //ambientTexture, diffuseTexture, specularTexture
        std::string type;
        std::string path;
    };

    struct Material
    {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
}

namespace str
{
    struct TextureInfo
    {
        std::string path;
        std::string type;
    };
}
