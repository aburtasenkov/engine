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

    Camera(glm::vec3 position = ORIGIN, glm::vec3 direction = NEGATIVE_Z_AXIS)
      :pos(position), front(direction), up(WORLD_UP), sensitivity(SENSITIVITY), yaw(YAW), pitch(PITCH)
    {
      right = glm::cross(direction, up);
      updateVectors();
    }

    void rotate(float offsetX, float offsetY) {
      yaw += offsetX * sensitivity;
      pitch += offsetY * sensitivity;

      if (pitch > MAX_PITCH) pitch = MAX_PITCH;
      if (pitch < MIN_PITCH) pitch = MIN_PITCH;

      updateVectors();
    }

    void movePos(CameraMovement direction, float cameraSpeed) {
      switch (direction) {
        case CameraMovement::FRONT:
        {
          pos += front* cameraSpeed;
          break;
        }
        case CameraMovement::BACK:
        {
          pos -= front * cameraSpeed;
          break;
        }
        case CameraMovement::RIGHT:
        {
          pos += right * cameraSpeed;
          break;
        }
        case CameraMovement::LEFT:
        {
          pos -= right * cameraSpeed;
          break;
        }
      }
    }

    glm::mat4 getViewMatrix() const {
      return glm::lookAt(pos, pos + front, up);
    }

  private:
    static constexpr glm::vec3 NEGATIVE_Z_AXIS = glm::vec3(0.0f, 0.0f, -1.0f);
    static constexpr glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    static constexpr float YAW = -90.0f;
    static constexpr float PITCH = 0.0f;

    static constexpr float SENSITIVITY = 0.1f;

    static constexpr float MAX_PITCH = 89.0f;
    static constexpr float MIN_PITCH = -89.0f;

    void updateVectors() {
      glm::vec3 direction;
      direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      direction.y = sin(glm::radians(pitch));
      direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      front = glm::normalize(direction);

      right = glm::normalize(glm::cross(front, WORLD_UP));
    }
};
