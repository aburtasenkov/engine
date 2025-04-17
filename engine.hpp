#pragma once

#include "source/dependancy/glad.h"
#include <GLFW/glfw3.h>
#include "source/dependancy/glm/glm.hpp"

#include "source/camera.hpp"
#include "source/shader.hpp"
#include "source/light.hpp"
#include "source/texture.hpp"
#include "source/constants.hpp"

#include <unordered_map>
#include <array>
#include <memory>

static void mouseMove_cb(GLFWwindow * window, double xPos, double yPos);
static void mouseScroll_cb(GLFWwindow * window, double offsetX, double offsetY);
static void processInput(GLFWwindow * window, Camera& camera);  

struct Variables {
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  float lastMousePosX = -1.0f;
  float lastMousePosY = -1.0f;
};

class Engine {
  public:
    static Engine * instance;

    GLFWwindow * window;

    Camera currentCam;
    std::unique_ptr<LightSource[]> lights;

    Variables variables;

    Engine()
      :currentCam()
    {
      instance = this;
      lights = std::make_unique<LightSource[]>(MAX_LIGHT_SOURCES); 
      if (!glfwInit()) {
        std::cerr << "ERROR::GLFW::NOT::INITIALIZED" << std::endl;
        glfwTerminate();
        exit(-1);
      }
    
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
      window = glfwCreateWindow(800, 600, "My Window", NULL, NULL);
      if (!window) {
        std::cerr << "ERROR::GLFW::WINDOW::CREATION::FAILED" << std::endl;
        glfwTerminate();
        exit(-1);
      }
      glfwMakeContextCurrent(window);
      
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR::GLAD::LOADER" << std::endl;
        exit(-1);
      }
    
      glViewport(0, 0, 800, 600);
      glClearColor(1, 1, 1, 1);
      glEnable(GL_DEPTH_TEST);
    
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPosCallback(window, mouseMove_cb);
      glfwSetScrollCallback(window, mouseScroll_cb);
      glfwSetFramebufferSizeCallback(window, 
        [](GLFWwindow *, int width, int height) {
          glViewport(0, 0, width, height);
        }
      );
    }

    ~Engine() {
      glfwTerminate();
    }

    bool run() {
      return !glfwWindowShouldClose(window);
    }

    void update() {
      glfwSwapBuffers(window);
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      processInput(window, currentCam);
    }
};

Engine * Engine::instance = nullptr;


static void mouseMove_cb(GLFWwindow * window, double xPos, double yPos) {

  if (Engine::instance->variables.lastMousePosX == -1.0f && Engine::instance->variables.lastMousePosY == -1.0f) {
    Engine::instance->variables.lastMousePosX = xPos;
    Engine::instance->variables.lastMousePosY = yPos;
    return;
  }

  float offsetX = xPos - Engine::instance->variables.lastMousePosX;
  float offsetY = Engine::instance->variables.lastMousePosY - yPos;

  Engine::instance->variables.lastMousePosX = xPos;
  Engine::instance->variables.lastMousePosY = yPos;

  Engine::instance->currentCam.rotate(offsetX, offsetY);
}

static void mouseScroll_cb(GLFWwindow * window, double offsetX, double offsetY) {
  Engine::instance->currentCam.fov -= (float)offsetY;

  if (Engine::instance->currentCam.fov > 45.0f) Engine::instance->currentCam.fov = 45.0f;
  else if (Engine::instance->currentCam.fov < 1.0f) Engine::instance->currentCam.fov = 1.0f;
}

static void processInput(GLFWwindow * window, Camera& camera) {
  std::unordered_map<unsigned int, CameraMovement> keyDirection {
    {GLFW_KEY_W, CameraMovement::FRONT},
    {GLFW_KEY_S, CameraMovement::BACK},
    {GLFW_KEY_D, CameraMovement::RIGHT},
    {GLFW_KEY_A, CameraMovement::LEFT}
  };

  float currentTime = (float)glfwGetTime();
  Engine::instance->variables.deltaTime = currentTime - Engine::instance->variables.lastFrame;
  Engine::instance->variables.lastFrame = currentTime;

  float cameraSpeed = 5.0f * Engine::instance->variables.deltaTime;

  for (auto key : {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_A}) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
      camera.movePos(keyDirection.at(key), cameraSpeed);
    }
  }
}
