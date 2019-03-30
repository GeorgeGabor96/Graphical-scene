#pragma once
#include "Model3D.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace enty
{
    class Entity
    {
    public:
        Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
        void draw(gps::Shader shaderProgram);
        void setRenderObject(gps::Model3D renderObj);
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getScale();

    protected:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
    private:
        gps::Model3D renderObj;
    };
}
