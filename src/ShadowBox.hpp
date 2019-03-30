#pragma once


#include "Shader.hpp"
#include <vector>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Config.h"
#include "glm/gtc/matrix_transform.hpp"

#define Z_NEAR 1.0f
#define Z_FAR 10.0f
#define DISTANCE 20.0f

namespace shadow
{
    glm::mat4 getOrtho(glm::vec3 position);
    glm::mat4 getLightSpaceTrMatrix(glm::vec3 lightDir, glm::vec3 lightPos, glm::vec3 cameraTarget);
}
