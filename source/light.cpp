#include "light.hpp"

LightSource::LightSource() {
  kind = LightKind::Undefined;
  vector = glm::vec3(1.0f);
  color = glm::vec3(1.0f);
}

LightSource::LightSource(LightKind k, glm::vec3 lightVector = ORIGIN, glm::vec3 lightColor = DEFAULT_COLOR)
  :kind(k), vector(lightVector), color(lightColor)
{    }

void LightSource::updateVec(glm::vec3 newVector) {
  vector = newVector;
}

void LightSource::updateColor(glm::vec3 newColor) {
  color = newColor;
}