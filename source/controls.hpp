#pragma once

#include "dependancy/glad.h"
#include <GLFW/glfw3.h>
#include "camera.hpp"

#include <unordered_map>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastMousePosX = -1.0f;
float lastMousePosY = -1.0f;

extern Camera currentCam;

static float fov = 45.0f;

static void mouseMove_cb(GLFWwindow * window, double xPos, double yPos) {

  if (lastMousePosX == -1.0f && lastMousePosY == -1.0f) {
    lastMousePosX = xPos;
    lastMousePosY = yPos;
    return;
  }

  float offsetX = xPos - lastMousePosX;
  float offsetY = lastMousePosY - yPos;

  lastMousePosX = xPos;
  lastMousePosY = yPos;

  currentCam.rotate(offsetX, offsetY);
}

static void mouseScroll_cb(GLFWwindow * window, double offsetX, double offsetY) {
  fov -= (float)offsetY;

  if (fov > 45.0f) fov = 45.0f;
  else if (fov < 1.0f) fov = 1.0f;
}

static void processInput(GLFWwindow * window, Camera& camera) {
  std::unordered_map<unsigned int, CameraMovement> keyDirection {
    {GLFW_KEY_W, CameraMovement::FRONT},
    {GLFW_KEY_S, CameraMovement::BACK},
    {GLFW_KEY_D, CameraMovement::RIGHT},
    {GLFW_KEY_A, CameraMovement::LEFT}
  };

  float currentTime = (float)glfwGetTime();
  deltaTime = currentTime - lastFrame;
  lastFrame = currentTime;

  float cameraSpeed = 5.0f * deltaTime;

  for (auto key : {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_A}) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
      camera.movePos(keyDirection.at(key), cameraSpeed);
    }
  }
}