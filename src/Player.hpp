#pragma once

#include "Config.h"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model3D.hpp"
#include "GLFW/glfw3.h"
#include "Terrain.hpp"
#include "Entity.hpp"
#include <math.h>

namespace enty
{
    class Player:public Entity
    {
    public:
        Player(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
        void update(bool pressW, bool pressS, bool pressA, bool pressD, bool pressSpace);
        glm::mat4 getModelMatrix();
        void move(float frameDuration, enty::Terrain terrain);

    private:
        float currentSpeed;
        float currentTurnSpeed;
        float currentUpwardsSpeed;
        bool jumping;
    };
}