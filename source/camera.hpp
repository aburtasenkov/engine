#pragma once

#include "dependancy/glad.h"
#include "dependancy/glm/glm.hpp"
#include "dependancy/glm/gtc/matrix_transform.hpp"

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
    static constexpr glm::vec3 NEGATIVE_Z_AXIS = glm::vec3(0.0f, 0.0f, -1.0f);
    static constexpr glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    static constexpr float YAW = -90.0f;
    static constexpr float PITCH = 0.0f;

    static constexpr float SENSITIVITY = 0.1f;

    static constexpr float MAX_PITCH = 89.0f;
    static constexpr float MIN_PITCH = -89.0f;

    void updateVectors();
};
