#include "ShadowBox.hpp"

namespace shadow
{
    glm::mat4 getOrtho(glm::vec3 position)
    {
        float minX = position.x - DISTANCE;
        float maxX = position.x + DISTANCE;
        float minY = position.y - DISTANCE;
        float maxY = position.y + DISTANCE;
        
        return glm::ortho(-DISTANCE, +DISTANCE, -DISTANCE, +DISTANCE, Z_NEAR, Z_FAR);
    }

    glm::mat4 getLightSpaceTrMatrix(glm::vec3 cameraPos, glm::vec3 cameraUp, glm::vec3 cameraTarget)
    {
        glm::mat4 lightView = glm::lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);
        glm::mat4 projection = getOrtho(cameraPos);

        return projection * lightView;
    }
}
