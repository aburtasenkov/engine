#pragma once

#include "dependancy/glm/glm.hpp"

class LightSource {
  public:
    glm::vec3 pos;
    glm::vec3 color;

    LightSource(glm::vec3 position = ORIGIN, glm::vec3 lightColor = DEFAULT_COLOR)
      :pos(position), color(lightColor)
    {    }

    void updatePos(glm::vec3 newPos) {
      pos = newPos;
    }

    void updateColor(glm::vec3 newColor) {
      color = newColor;
    }

  private:
    static constexpr glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
    static constexpr glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
};

LightSource currentLight;