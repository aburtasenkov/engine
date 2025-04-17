#include "camera.hpp"

Camera::Camera(glm::vec3 position = ORIGIN, glm::vec3 direction = NEGATIVE_Z_AXIS)
  :pos(position), front(direction), up(WORLD_UP), sensitivity(SENSITIVITY), yaw(YAW), pitch(PITCH)
{
  right = glm::cross(direction, up);
  updateVectors();
}

void Camera::rotate(float offsetX, float offsetY) {
  yaw += offsetX * sensitivity;
  pitch += offsetY * sensitivity;

  if (pitch > MAX_PITCH) pitch = MAX_PITCH;
  if (pitch < MIN_PITCH) pitch = MIN_PITCH;

  updateVectors();
}

void Camera::movePos(CameraMovement direction, float cameraSpeed) {
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

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(pos, pos + front, up);
}

void Camera::updateVectors() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);

  right = glm::normalize(glm::cross(front, WORLD_UP));
}