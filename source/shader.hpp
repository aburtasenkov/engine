#pragma once

#include "dependancy/glad.h"
#include "GLFW/glfw3.h"
#include "dependancy/glm/gtc/type_ptr.hpp"

#include "camera.hpp"
#include "light.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

class Shader {
  public:
    unsigned int shaderID;

    Shader(std::filesystem::path vertexShader, std::filesystem::path fragmentShader) {
      shaderID = glCreateProgram();

      // Get compiled shaders
      unsigned int vShader = compileShader(vertexShader, GL_VERTEX_SHADER);
      unsigned int fShader = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
    
      // link shaders to shaderID
      glAttachShader(shaderID, vShader);
      glAttachShader(shaderID, fShader);
      glLinkProgram(shaderID);
    
      // Output errors
      int status;
      glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
      if (!status) {
        char infoLog[512];
        glGetProgramInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR DURING SHADER LINKING" << std::endl
                  << infoLog << std::endl;
      }
    
      glDeleteShader(vShader);
      glDeleteShader(fShader);
    }

    ~Shader() {
      glDeleteProgram(shaderID);
    }

    void use() {
      glUseProgram(shaderID);
    }

    void generateModelMatrix(float angle, glm::vec3 axis, glm::vec3 pos) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, pos);
      model = glm::rotate(model, angle, axis);
      bindMatrix("model", model);
    }
    
    void generateViewMatrix(Camera& camera) {
      bindMatrix("view", camera.getViewMatrix());
    }
    
    void generateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
      glm::mat4 projection{1.0f};
      projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
      bindMatrix("projection", projection);
    }

    void setUniformVec3(std::string uniform, glm::vec3 vector) {
      int uniformLocation = glGetUniformLocation(shaderID, uniform.c_str());
      if (uniformLocation == -1) {
        std::cerr << "Error getting " << uniform << " uniform location" << std::endl;
      }
      glUniform3fv(uniformLocation, 1, glm::value_ptr(vector));
    }

    void setUniformFloat(std::string uniform, float value) {
      int uniformLocation = glGetUniformLocation(shaderID, uniform.c_str());
      if (uniformLocation == -1) {
        std::cerr << "Error getting " << uniform << " uniform location" << std::endl;
      }
      glUniform1f(uniformLocation, value);
    }

    void updateFragmentShaderUniforms(Camera& camera, LightSource& light) {
      setUniformVec3("light.position", light.pos);
      setUniformVec3("light.ambient", glm::vec3(0.1, 0.1, 0.1));
      setUniformVec3("light.diffuse", glm::vec3(0.5, 0.5, 0.5));
      setUniformVec3("light.specular", glm::vec3(1.0, 1.0, 1.0));

      setUniformFloat("light.constant", light.constant);
      setUniformFloat("light.linear", light.linear);
      setUniformFloat("light.quadratic", light.quadratic);

      setUniformVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
      setUniformFloat("material.shininess", 32.0f);
      
      setUniformVec3("CameraPos", camera.pos);
    }

  private:
    unsigned int compileShader(std::filesystem::path filename, GLenum shaderType) {
      unsigned int shader = glCreateShader(shaderType);
    
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
      glShaderSource(shader, 1, &cCode, NULL);
      glCompileShader(shader);
    
      // Check for compilation errors
      int status;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
      if (!status) {
        char infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR DURING SHADER COMPILATION: " << filename << std::endl
                  << infoLog << std::endl;
      }
      return shader;
    }
    
    void bindMatrix(const char * uniformName, glm::mat4 matrix) {
      int uniformId = glGetUniformLocation(shaderID, uniformName);
      if (uniformId == -1) {
        std::cerr << "Error getting " << uniformName << " 's location" << std::endl;
      }
      glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(matrix));
    }
};
