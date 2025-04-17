#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

const int MAX_LIGHT_SOURCES = 8;

struct Material {
  sampler2D diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  int kind;
  // kind defines what kind of light source it is
  // 0 - Undefined --> pass
  // 1 - Directional (usually indexed at 0)
  // 2 - Point

  vec3 vector;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform Material material;
uniform Light light[MAX_LIGHT_SOURCES];

uniform vec3 CameraPos;

out vec4 Color;

float lightAttenuation(float contant, float linear, float quadratic, float distance);

vec3 ambientLight(vec3 fragColor, vec3 ambient);
vec3 diffuseLight(vec3 fragColor, vec3 normal, vec3 lightDirection, vec3 diffuse);
vec3 specularLight(vec3 fragColor, vec3 normal, vec3 lightDirection, vec3 viewDirection, float shininess, vec3 specular);

vec3 directionalLight(vec3 fragColor, vec3 normal, Light light);
vec3 pointLight(vec3 fragColor, vec3 normal, Light light);

void main() {
  vec3 normal = normalize(Normal);
  vec3 fragColor = vec3(texture(material.diffuse, TexCoord));

  vec3 result = vec3(0.0);

  for (int i = 0; i < MAX_LIGHT_SOURCES; ++i) {
    vec3 lightEffect = vec3(0.0);
    if (light[i].kind == 0) continue;
    if (light[i].kind == 1) lightEffect = directionalLight(fragColor, normal, light[i]);
    if (light[i].kind == 2) lightEffect = pointLight(fragColor, normal, light[i]);
    result += lightEffect;
  }

  Color = vec4(result, 1.0);
}

float lightAttenuation(float constant, float linear, float quadratic, float d) {
  return 1.0 / (constant + 
              (linear * d) +
              (quadratic * d * d));
}

vec3 ambientLight(vec3 fragColor, vec3 ambient) 
// ambient is the light sources ambient value
{
  return fragColor * ambient;
}

vec3 diffuseLight(vec3 fragColor, vec3 normal, vec3 lightDirection, vec3 diffuse) 
// lightDirection points from light source to the fragment
// diffuse is lights diffuse value
{
  float diffuseValue = max(dot(-lightDirection, normal), 0.0);
  return fragColor * diffuse * diffuseValue;
}

vec3 specularLight(vec3 fragColor, vec3 normal, vec3 lightDirection, vec3 viewDirection, float shininess, vec3 specular)
// lightDirection points from light source to the fragment
// viewDirection points from camera to the fragment
// specular is lights specular value
{
  vec3 reflected = reflect(lightDirection, normal);
  float specularValue = pow(max(dot(reflected, -viewDirection), 0.0), shininess);
  return specular * specularValue;
}

vec3 directionalLight(vec3 fragColor, vec3 normal, Light light) {
  vec3 lightDirection = normalize(light.vector);
  vec3 viewDirection = normalize(FragPos - CameraPos);

  vec3 ambient = ambientLight(fragColor, light.ambient);
  vec3 diffuse = diffuseLight(fragColor, normal, lightDirection, light.diffuse);
  vec3 specular = specularLight(fragColor, normal, lightDirection, viewDirection, material.shininess, light.specular);

  return ambient + diffuse + specular;
}

vec3 pointLight(vec3 fragColor, vec3 normal, Light light) {
  vec3 lightDirection = normalize(FragPos - light.vector);
  vec3 viewDirection = normalize(FragPos - CameraPos);

  vec3 ambient = ambientLight(fragColor, light.ambient);
  vec3 diffuse = diffuseLight(fragColor, normal, lightDirection, light.diffuse);
  vec3 specular = specularLight(fragColor, normal, lightDirection, viewDirection, material.shininess, light.specular);

  float attenuation = lightAttenuation(light.constant, light.linear, light.quadratic, distance(FragPos, light.vector));

  return (ambient + diffuse + specular) * attenuation;
}
