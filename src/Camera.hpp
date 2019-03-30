//
//  Camera.hpp
//
//  Created by CGIS on 28/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/transform.hpp"
#include "Config.h"

/*
*   -- Aparent FINISH --   
*/

namespace gps {
    
    class Camera
    {
    public:
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget);
        glm::mat4 getViewMatrix();
        void move(bool pressW, bool pressS, bool pressA, bool pressD, bool pressR, bool pressF);
        void rotate(float pitch, float yaw);
        glm::vec3 getPosition();
        void addDistanceToObj(float distance);
        void focusObject(glm::vec3 objPosition, glm::vec3 objRotation, float pitch, float yaw);
        void addHeight(float height);
        void invertCameraTarget();
        glm::vec3 getCameraTarget();
        glm::vec3 getUp();
    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUp;
        float yaw;
        float pitch;
        float distanceFromObject;
    };
    
}

