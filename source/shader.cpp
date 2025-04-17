#include "shader.hpp"

void checkUniformNotFoundError(std::string uniformName, int uniformLocation) {
  if (uniformLocation == -1) std::cerr << "Error getting " << uniformName << " uniform location" << std::endl; 
}

Shader::Shader(std::filesystem::path vertexShader, std::filesystem::path fragmentShader) {
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

Shader::~Shader() {
  glDeleteProgram(shaderID);
}

void Shader::use() {
  glUseProgram(shaderID);
}

void Shader::generateModelMatrix(float angle, glm::vec3 axis, glm::vec3 pos) {
  glm::mat4 model{1.0f};
  model = glm::translate(model, pos);
  model = glm::rotate(model, angle, axis);
  bindMatrix("model", model);
}

void Shader::generateViewMatrix(Camera& camera) {
  bindMatrix("view", camera.getViewMatrix());
}

void Shader::generateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
  glm::mat4 projection{1.0f};
  projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
  bindMatrix("projection", projection);
}

void Shader::setUniformVec3(std::string uniformName, glm::vec3 vector) {
  int uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
  checkUniformNotFoundError(uniformName, uniformLocation);
  glUniform3fv(uniformLocation, 1, glm::value_ptr(vector));
}

void Shader::setUniformFloat(std::string uniformName, float value) {
  int uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
  checkUniformNotFoundError(uniformName, uniformLocation);
  glUniform1f(uniformLocation, value);
}

void Shader::setUniformInteger(std::string uniformName, int value) {
  int uniformLocation = glGetUniformLocation(shaderID, uniformName.c_str());
  checkUniformNotFoundError(uniformName, uniformLocation);
  glUniform1i(uniformLocation, value);
}

void Shader::updateFragmentShaderUniforms(Camera& currentCam, LightSource * array, size_t sz) {
  for (int i = 0; i < sz; ++i) {
    if (array[i].kind != LightKind::Undefined) updateFragmentShaderLightUniforms(i, array[i]);
    std::cout << static_cast<int>(array[i].kind) << "\n";
  }

  setUniformVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
  setUniformFloat("material.shininess", 32.0f);
  
  setUniformVec3("CameraPos", currentCam.pos);
}

void Shader::updateFragmentShaderLightUniforms(size_t index, LightSource& light) {
  std::string i = std::to_string(index);
  setUniformInteger("light[" + i + "].kind", static_cast<int>(light.kind));
  setUniformVec3("light[" + i + "].vector", light.vector);
  setUniformVec3("light[" + i + "].ambient", glm::vec3(0.1, 0.1, 0.1));
  setUniformVec3("light[" + i + "].diffuse", glm::vec3(0.5, 0.5, 0.5));
  setUniformVec3("light[" + i + "].specular", glm::vec3(1.0, 1.0, 1.0));

  setUniformFloat("light[" + i + "].constant", light.constant);
  setUniformFloat("light[" + i + "].linear", light.linear);
  setUniformFloat("light[" + i + "].quadratic", light.quadratic);
}

unsigned int Shader::compileShader(std::filesystem::path filename, GLenum shaderType) {
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

void Shader::bindMatrix(const char * uniformName, glm::mat4 matrix) {
  int uniformLocation = glGetUniformLocation(shaderID, uniformName);
  checkUniformNotFoundError(uniformName, uniformLocation);
  glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}
