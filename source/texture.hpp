#define STB_IMAGE_IMPLEMENTATION
#include "dependancy/stb_image.h"
#include "dependancy/glad.h"

#include <iostream>

static unsigned int loadTextureJPG(const char * filename, unsigned int texturePos) {
  int width, height, channels_count;
  unsigned char * img_data = stbi_load(filename, &width, &height, &channels_count, 0);

  if (!img_data) {
    std::cerr << "ERROR::FAILED::LOADING::TEXTURE" << std::endl;
    exit(-1);
  }

  unsigned int texture;
  glGenTextures(1, &texture);
  glActiveTexture(texturePos);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // Determine format based on number of channels
  GLenum format = (channels_count == 4) ? GL_RGBA : GL_RGB;

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img_data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(img_data);

  return texture;
}
