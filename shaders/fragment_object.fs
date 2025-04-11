#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Material {
  sampler2D diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 CameraPos;

out vec4 Color;

void main() {
  vec3 normal = normalize(Normal);
  vec3 lightDirection = normalize(light.position - FragPos);

  vec3 fragColor = vec3(texture(material.diffuse, TexCoord));
  
  // Light Attentuation
  float d = distance(FragPos, light.position);
  float numerator = 1.0;
  float denumerator = light.constant + light.linear * d * light.  quadratic * d * d;

  float attentuation = numerator / denumerator;

  // Ambient Light
  vec3 ambient = fragColor * light.ambient;

  // Diffuse Light
  float diffuseValue = max(dot(lightDirection, normal), 0.0);
  vec3 diffuse = (fragColor * diffuseValue) * light.diffuse;

  // Specular Light
  vec3 reflected = reflect(-lightDirection, normal);
  vec3 viewDirection = normalize(CameraPos - FragPos);
  float spec = pow(max(dot(reflected, viewDirection), 0.0), material.shininess);
  vec3 specular = (material.specular * spec) * light.specular;

  ambient *= attentuation;
  diffuse *= attentuation;
  specular *= attentuation;

  vec3 result = ambient + diffuse + specular;

  Color = vec4(result, 1.0);
}