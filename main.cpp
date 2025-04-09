#include "engine.hpp"

int main() {
  Engine engine;

  float vertices[] = {
    // positions - 3              // normals - 3            // texture coords - 2
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
  Shader objectShader{"shaders/vertex_object.vs", "shaders/fragment_object.fs"};

  // Load Light Shader
  Shader lightShader{"shaders/vertex_object.vs", "shaders/fragment_light.fs"};

  // Load texture
  loadTextureJPG("textures/gold.jpg", GL_TEXTURE0);
  int textureUniformId = glGetUniformLocation(objectShader.shaderID, "material.diffuse");
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

  while (engine.run()) {
    engine.currentLight.updatePos(glm::vec3{(float)cos(glfwGetTime()) * 10, 2.0f, (float)sin(glfwGetTime()) * 10});
  
    objectShader.updateFragmentShaderUniforms(engine.currentCam, engine.currentLight);
    
    glBindVertexArray(lightVAO);
    lightShader.use();
    lightShader.generateModelMatrix(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f), engine.currentLight.pos);
    lightShader.generateViewMatrix(engine.currentCam);
    lightShader.generateProjectionMatrix(glm::radians(engine.currentCam.fov), 800.0f / 600.0f, 0.01f, 100.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(objectVAO);
    objectShader.use();
    objectShader.generateViewMatrix(engine.currentCam);
    objectShader.generateProjectionMatrix(glm::radians(engine.currentCam.fov), 800.0f / 600.0f, 0.01f, 100.0f);
    for (int i = 0; i < 10; ++i){
      objectShader.generateModelMatrix(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f), cubePositions[i]);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    engine.update();
  }

  glDeleteBuffers(1, &cube);

  glDeleteVertexArrays(1, &objectVAO);
  glDeleteVertexArrays(1, &lightVAO);
  
  return 0;
}
