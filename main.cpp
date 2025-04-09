#include "source/dependancy/glad.h"
#include <GLFW/glfw3.h>
#include "source/dependancy/glm/glm.hpp"

#include "source/camera.hpp"
#include "source/shader.hpp"
#include "source/light.hpp"
#include "source/controls.hpp"
#include "source/texture.hpp"

extern float fov;

extern Camera currentCam;
extern LightSource currentLight;

static GLFWwindow * initWindow() {
  if (!glfwInit()) {
    std::cerr << "ERROR::GLFW::NOT::INITIALIZED" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow * window = glfwCreateWindow(800, 600, "My Window", NULL, NULL);
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

  return window;
}

int main() {
  GLFWwindow * window = initWindow();

  float vertices[] = {
    // positions - 3 // normals - 3 // texture coords - 2
    -0.5f, -0.5f, -0.5f,          0.0f, 0.0f, -1.0f,        0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,           0.0f, 0.0f, -1.0f,        1.0f, 0.0f,
    0.5f, 0.5f, -0.5f,            0.0f, 0.0f, -1.0f,        1.0f, 1.0f,
    0.5f, 0.5f, -0.5f,            0.0f, 0.0f, -1.0f,        1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f,           0.0f, 0.0f, -1.0f,        0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,          0.0f, 0.0f, -1.0f,        0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,           0.0f, 0.0f, 1.0f,         0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,            0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,             0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,             0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,            0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,           0.0f, 0.0f, 1.0f,         0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f,            -1.0f, 0.0f, 0.0f,        1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,           -1.0f, 0.0f, 0.0f,        1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,          -1.0f, 0.0f, 0.0f,        0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,          -1.0f, 0.0f, 0.0f,        0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,           -1.0f, 0.0f, 0.0f,        0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,            -1.0f, 0.0f, 0.0f,        1.0f, 0.0f,
    
    0.5f, 0.5f, 0.5f,             1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
    0.5f, 0.5f, -0.5f,            1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,           1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,           1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,            1.0f, 0.0f, 0.0f,         0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,             1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,          0.0f, -1.0f, 0.0f,        0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,           0.0f, -1.0f, 0.0f,        1.0f, 1.0f,
    0.5f, -0.5f, 0.5f,            0.0f, -1.0f, 0.0f,        1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,            0.0f, -1.0f, 0.0f,        1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,           0.0f, -1.0f, 0.0f,        0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,          0.0f, -1.0f, 0.0f,        0.0f, 1.0f,
  
    -0.5f, 0.5f, -0.5f,           0.0f, 1.0f, 0.0f,         0.0f, 1.0f,
    0.5f, 0.5f, -0.5f,            0.0f, 1.0f, 0.0f,         1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,             0.0f, 1.0f, 0.0f,         1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,             0.0f, 1.0f, 0.0f,         1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,            0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,           0.0f, 1.0f, 0.0f,         0.0f, 1.0f
  };

  // Object Data
  unsigned int objectVAO;
  glGenVertexArrays(1, &objectVAO);
  glBindVertexArray(objectVAO);

  unsigned int cube;
  glGenBuffers(1, &cube);
  glBindBuffer(GL_ARRAY_BUFFER, cube);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void *)(sizeof(float) * 3));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void *)(sizeof(float) * 6));
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, cube);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void *)0);
  glEnableVertexAttribArray(0);

  // Load  Shader
  unsigned int objectShader = linkShaders("shaders/vertex_object.vs", "shaders/fragment_object.fs");

  // Load Light Shader
  unsigned int lightShader = linkShaders("shaders/vertex_object.vs", "shaders/fragment_light.fs");

  // Load texture
  loadTextureJPG("textures/gold.jpg", GL_TEXTURE0);
  int textureUniformId = glGetUniformLocation(objectShader, "material.diffuse");
  glUniform1i(textureUniformId, 0);

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
  };

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    currentLight.updatePos(glm::vec3{(float)cos(glfwGetTime()) * 10, 2.0f, (float)sin(glfwGetTime()) * 10});
    
    processInput(window, currentCam);
    updateFragmentShaderUniforms(objectShader, currentCam, currentLight);
    
    glUseProgram(lightShader);
    glBindVertexArray(lightVAO);
    generateModelMatrix(lightShader, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f), currentLight.pos);
    generateViewMatrix(lightShader, currentCam);
    generateProjectionMatrix(lightShader, glm::radians(fov), 800.0f / 600.0f, 0.01f, 100.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUseProgram(objectShader);
    glBindVertexArray(objectVAO);
    generateViewMatrix(objectShader, currentCam);
    generateProjectionMatrix(objectShader, glm::radians(fov), 800.0f / 600.0f, 0.01f, 100.0f);
    for (int i = 0; i < 10; ++i){
      generateModelMatrix(objectShader, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f), cubePositions[i]);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &cube);

  glDeleteVertexArrays(1, &objectVAO);
  glDeleteProgram(objectShader);

  glDeleteVertexArrays(1, &lightVAO);
  glDeleteProgram(lightShader);

  glfwTerminate();
  
  return 0;
}
