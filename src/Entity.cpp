#include "Entity.hpp"

namespace enty
{
        Entity::Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
        {
            this->position = position;
            this->scale = scale;
            this->rotation = rotation;
        }
        
        void Entity::draw(gps::Shader shaderProgram)
        {
            renderObj.Draw(shaderProgram);
        }

        void Entity::setRenderObject(gps::Model3D renderObj)
        {
            this->renderObj = renderObj;
        }

        glm::vec3 Entity::getPosition()
        {
            return position;
        }

        glm::vec3 Entity::getRotation()
        {
            return rotation;
        }

        glm::vec3 Entity::getScale()
        {
            return scale;
        }
}