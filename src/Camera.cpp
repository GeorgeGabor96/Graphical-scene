//
//  Camera.cpp
//  Lab5
//
//  Created by CGIS on 28/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#include "Camera.hpp"
#include <stdio.h>

namespace gps {
    
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget)
    {
        this->yaw = 0;
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraDirection, this->cameraUp));
        this->distanceFromObject = 50;
    }

    void Camera::addHeight(float height)
    {
        cameraPosition.y += height;
    }

    void Camera::invertCameraTarget()
    {
        cameraDirection.y = -cameraDirection.y;
    }

    glm::vec3 Camera::getCameraTarget()
    {
        return cameraTarget;
    }

    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(cameraPosition, cameraPosition + cameraDirection , cameraUp);
    }
    
    void Camera::move(bool pressW, bool pressS, bool pressA, bool pressD, bool pressR, bool pressF)
    {
        if (true == pressW)
        {
            cameraPosition += cameraDirection * CAMERA_MOVEMENT_SPEED;
        }
        else if (true == pressS)
        {
            cameraPosition -= cameraDirection * CAMERA_MOVEMENT_SPEED;
        }
                
        if (true == pressD)
        {
            cameraPosition += cameraRightDirection * CAMERA_MOVEMENT_SPEED;
        }        
        else if (true == pressA)
        {
            cameraPosition -= cameraRightDirection * CAMERA_MOVEMENT_SPEED;
        }

        if (true == pressR)
        {
            cameraPosition += cameraUp * CAMERA_MOVEMENT_SPEED;
        }
        else if (true == pressF)
        {
            cameraPosition -= cameraUp * CAMERA_MOVEMENT_SPEED;
        }
    }
    
    void Camera::rotate(float pitch, float yaw)
    {
        glm::mat3 horizontallyRotation = glm::mat3(glm::rotate(glm::radians(-yaw * 50), cameraUp));
        glm::mat3 verticalRotation = glm::mat3(glm::rotate(glm::radians(-pitch * 50), cameraRightDirection));
        
        cameraDirection = glm::normalize(horizontallyRotation * cameraDirection);
        cameraDirection = glm::normalize(verticalRotation * cameraDirection);
        cameraRightDirection = glm::normalize(horizontallyRotation * cameraRightDirection);
        cameraUp = glm::normalize(verticalRotation * cameraUp);
    }

    glm::vec3 Camera::getPosition()
    {
        return this->cameraPosition;
    }

    glm::vec3 Camera::getUp()
    {
        return this->cameraUp;
    }

    void Camera::addDistanceToObj(float distance)
    {
        distanceFromObject += distance;
        if (distanceFromObject < 3.0f)
        {
            distanceFromObject = 3.0f;
        }
        else if (distanceFromObject > 100.0f)
        {
            distanceFromObject = 100.0f;
        }
    }

    void Camera::focusObject(glm::vec3 objPosition, glm::vec3 objRotation, float yaw1, float pitch1)
    {
        yaw -= yaw1 * 100;
        pitch += pitch1 * 100;

        float verticalDistance = distanceFromObject * sin(glm::radians(pitch));
        float horizontalDistance = distanceFromObject * cos(glm::radians(pitch));

        float theta = objRotation.y + yaw;
        float xOffset = horizontalDistance * sin(glm::radians(theta));
        float zOffset = horizontalDistance * cos(glm::radians(theta));
   
        cameraPosition.z = objPosition.z - zOffset;
        cameraPosition.x = objPosition.x - xOffset;
        objPosition.y += PLAYER_CENTER_HEIGHT;
        cameraPosition.y = objPosition.y + verticalDistance;
        cameraDirection = glm::normalize(objPosition - cameraPosition);

        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRightDirection = glm::normalize(glm::cross(cameraDirection, cameraUp));
    }
}
