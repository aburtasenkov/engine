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
#include <type_traits>

void checkUniformNotFoundError(std::string uniformName, int uniformId);

class Shader {
  public:
    unsigned int shaderID;

    Shader(std::filesystem::path vertexShader, std::filesystem::path fragmentShader);
    ~Shader();
    void use();

    void generateModelMatrix(float angle, glm::vec3 axis, glm::vec3 pos);    
    void generateViewMatrix(Camera& camera);    
    void generateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

    void setUniformVec3(std::string uniform, glm::vec3 vector);
    void setUniformFloat(std::string uniform, float value);
    void setUniformInteger(std::string uniform, int value);

    void updateFragmentShaderUniforms(Camera& currentCam, LightSource * array, size_t sz);
    void updateFragmentShaderLightUniforms(size_t index, LightSource& light);

  private:
    unsigned int compileShader(std::filesystem::path filename, GLenum shaderType);

    void bindMatrix(const char * uniformName, glm::mat4 matrix);
};
