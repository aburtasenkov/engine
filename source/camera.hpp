#pragma once

#include "dependancy/glad.h"
#include "dependancy/glm/glm.hpp"
#include "dependancy/glm/gtc/matrix_transform.hpp"

#include "constants.hpp"

#include <iostream>

enum class CameraMovement {
  RIGHT, LEFT, FRONT, BACK
};

class Camera {
  public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    float sensitivity;

    float pitch;
    float yaw;

    float fov = 45.0f;

    Camera(glm::vec3 position = ORIGIN, glm::vec3 direction = NEGATIVE_Z_AXIS);
    void rotate(float offsetX, float offsetY);
    void movePos(CameraMovement direction, float cameraSpeed);
    glm::mat4 getViewMatrix() const;

  private:
    void updateVectors();
};
