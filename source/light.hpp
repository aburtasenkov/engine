#pragma once

#include "dependancy/glm/glm.hpp"
#include "constants.hpp"

enum class LightKind : int {
  Undefined = 0, Directional, Point
};

class LightSource {
  public:
    LightKind kind;

    glm::vec3 vector;
    glm::vec3 color;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    LightSource();
    LightSource(LightKind k, glm::vec3 lightVector = ORIGIN, glm::vec3 lightColor = DEFAULT_COLOR);

    void updateVec(glm::vec3 newVector);
    void updateColor(glm::vec3 newColor);
};
