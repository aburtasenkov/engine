#pragma once

#include "dependancy/glad.h"
#include "GLFW/glfw3.h"
#include "dependancy/glm/gtc/type_ptr.hpp"

#include "camera.hpp"
#include "light.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

unsigned int compileShader(const std::string& filename, GLenum shaderType) {
  unsigned int vShader = glCreateShader(shaderType);

  // Get GLSL code in a C-style string
  std::ifstream ifs{filename};
  if (!ifs) {
    std::cerr << "ERROR CANNOT OPEN FILE " << filename << std::endl;
  }
  std::ostringstream ofs;
  ofs << ifs.rdbuf();
  std::string glslCode = ofs.str();
  const char * cCode = glslCode.c_str();

  // Compile
  glShaderSource(vShader, 1, &cCode, NULL);
  glCompileShader(vShader);

  // Check for compilation errors
  int status;
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
  if (!status) {
    char infoLog[512];
    glGetShaderInfoLog(vShader, sizeof(infoLog), NULL, infoLog);
    std::cerr << "ERROR DURING SHADER COMPILATION: " << filename << std::endl
              << infoLog << std::endl;
  }
  return vShader;
}

unsigned int linkShaders(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  
  // Get compiled shaders
  unsigned int vShader = compileShader(vertexShader, GL_VERTEX_SHADER);
  unsigned int fShader = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

  // link shaders to program
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);
  glLinkProgram(program);

  // Output errors
  int status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    char infoLog[512];
    glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
    std::cerr << "ERROR DURING SHADER LINKING" << std::endl
              << infoLog << std::endl;
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  
  return program;
}

void bindMatrix(unsigned int shader, const char * uniformName, glm::mat4 matrix) {
  int uniformId = glGetUniformLocation(shader, uniformName);
  if (uniformId == -1) {
    std::cerr << "Error getting " << uniformName << " 's location" << std::endl;
  }
  glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(matrix));
}

void generateModelMatrix(unsigned int shader, float angle, glm::vec3 axis, glm::vec3 pos) {
  glm::mat4 model{1.0f};
  model = glm::rotate(model, angle, axis);
  model = glm::translate(model, pos);
  bindMatrix(shader, "model", model);
}

void generateViewMatrix(unsigned int shader, Camera& camera) {
  bindMatrix(shader, "view", camera.getViewMatrix());
}

void generateProjectionMatrix(unsigned int shader, float fov, float aspectRatio, float nearPlane, float farPlane) {
  glm::mat4 projection{1.0f};
  projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
  bindMatrix(shader, "projection", projection);
}

void updateFragmentShaderUniforms(unsigned int shader, Camera& camera, LightSource& light) {
  int lightPosUniform = glGetUniformLocation(shader, "light.position");
  if (!lightPosUniform) {
    std::cerr << "Error getting light.position uniform location" << std::endl;
  }
  glUniform3fv(lightPosUniform, 1, glm::value_ptr(light.pos));

  int lightAmbient = glGetUniformLocation(shader, "light.ambient");
  if (!lightAmbient) {
    std::cerr << "Error getting light.ambient uniform location" << std::endl;
  }
  glUniform3fv(lightAmbient, 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));

  int lightDiffuse = glGetUniformLocation(shader, "light.diffuse");
  if (!lightDiffuse) {
    std::cerr << "Error getting light.diffuse uniform location" << std::endl;
  }
  glUniform3fv(lightDiffuse, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));

  int lightSpecular = glGetUniformLocation(shader, "light.specular");
  if (!lightSpecular) {
    std::cerr << "Error getting light.specular uniform location" << std::endl;
  }
  glUniform3fv(lightSpecular, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

  int materialSpecular = glGetUniformLocation(shader, "material.specular");
  if (!materialSpecular) {
    std::cerr << "Error getting material.specular uniform location" << std::endl;
  }
  glUniform3fv(materialSpecular, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

  int materialShininess = glGetUniformLocation(shader, "material.shininess");
  if (!materialShininess) {
    std::cerr << "Error getting material.shininess uniform location" << std::endl;
  }
  glUniform1f(materialShininess, 32.0f);

  int cameraPos = glGetUniformLocation(shader, "CameraPos");
  if (!cameraPos) {
    std::cerr << "Error getting CameraPos uniform location" << std::endl;
  }
  glUniform3fv(cameraPos, 1, glm::value_ptr(camera.pos));
}