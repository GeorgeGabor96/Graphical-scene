#include "Player.hpp"

namespace enty
{
    Player::Player(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation):Entity(position, scale, rotation)
    {
        this->currentSpeed = 0;
        this->currentTurnSpeed = 0;
        this->currentUpwardsSpeed = 0;
        this->jumping = false;
        this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::mat4 Player::getModelMatrix()
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

        return model;
    }

    void Player::move(float frameDuration, enty::Terrain terrain)
    {
        rotation.y += currentTurnSpeed * frameDuration;
        float distance = currentSpeed * frameDuration;
        float dx = (float)(distance * sin(glm::radians(rotation.y)));
        float dz = (float)(distance * cos(glm::radians(rotation.y)));
        position.x += dx;
        position.z += dz;
        
        currentUpwardsSpeed += GRAVITY * frameDuration;
        position.y += currentUpwardsSpeed * frameDuration;
        float terrainHeight = terrain.getTerrainHeight(this->position.x, this->position.z);

        if (position.y < terrainHeight)
        {
            currentUpwardsSpeed = 0.0f;
            position.y = terrainHeight;
            jumping = false;
        }
    }

    void Player::update(bool pressW, bool pressS, bool pressA, bool pressD, bool pressSpace)
    {
        if (pressW == true)
        {
            currentSpeed = PLAYER_RUN_SPEED;
        }
        else if (pressS == true)
        {
            currentSpeed = -PLAYER_RUN_SPEED;
        }
        else
        {
            currentSpeed = 0;
        }

        if (pressA == true)
        {
            currentTurnSpeed = PLAYER_TURN_SPEED;
        }
        else if (pressD == true)
        {
            currentTurnSpeed = -PLAYER_TURN_SPEED;
        }
        else
        {
            currentTurnSpeed = 0;
        }
        
        if (true == pressSpace && false == jumping)
        {
            currentUpwardsSpeed = PLAYER_JUMP_POWER;
            jumping = true;
        }
    }


}