#version 330 core

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

struct DirectionalLight {
   vec3 direction;

   vec3 diffuse;
   vec3 specular;
};

struct PointLight {
   vec3 position;

   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

struct SpotLight {
   vec3 position;
   vec3 direction;
   float innerCutoff;
   float outerCutoff;

   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

out vec4 FragColor;

uniform Material material;
uniform vec3 camPos;

uniform DirectionalLight directionalLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(camPos - FragPos);

   vec3 result = vec3(0, 0, 0);

   // Process all lights
   result += CalculateDirectionalLight(directionalLight, norm, viewDir);
   for (int i = 0; i < NR_POINT_LIGHTS; i++) {
      result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);
   }
   result += CalculateSpotLight(spotLight, norm, FragPos, viewDir);

   // ambient lighting
   vec3 ambient = 0.2 * vec3(texture(material.diffuse, TexCoords));
   result += ambient;

   FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
   vec3 lightDir = normalize(-light.direction);

   // diffuse
   float diffuseStrength = max(0.0, dot(lightDir, normal));
   vec3 diffuse = diffuseStrength * light.diffuse * vec3(texture(material.diffuse, TexCoords));

   // specular
   vec3 reflectedDirection = reflect(-lightDir, normal);
   float specularStrengh = pow(max(dot(viewDir, reflectedDirection), 0), material.shininess);
   vec3 specular = specularStrengh * light.specular * vec3(texture(material.specular, TexCoords));

   return (diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
   vec3 lightDir = normalize(light.position - fragPos);

   // diffuse
   float diffuseStrength = max(0.0, dot(lightDir, normal));
   vec3 diffuse = diffuseStrength * light.diffuse * vec3(texture(material.diffuse, TexCoords));

   // specular
   vec3 reflectedDirection = reflect(-lightDir, normal);
   float specularStrengh = pow(max(dot(viewDir, reflectedDirection), 0), material.shininess);
   vec3 specular = specularStrengh * light.specular * vec3(texture(material.specular, TexCoords));

   // attenuation
   float lightDistance = length(light.position - fragPos);
   float attenuation = 1 / (light.constant + light.linear * lightDistance + light.quadratic * lightDistance * lightDistance);

   return (diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
   vec3 lightDir = normalize(light.position - fragPos);

   // intensity
   float theta = dot(lightDir, normalize(-light.direction));
   float intensity = clamp((theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);

   // diffuse
   float diffuseStrength = max(0.0, dot(lightDir, normal));
   vec3 diffuse = diffuseStrength * light.diffuse * vec3(texture(material.diffuse, TexCoords));

   // specular
   vec3 reflectedDirection = reflect(-lightDir, normal);
   float specularStrengh = pow(max(dot(viewDir, reflectedDirection), 0), material.shininess);
   vec3 specular = specularStrengh * light.specular * vec3(texture(material.specular, TexCoords));

   // attenuation
   float lightDistance = length(light.position - fragPos);
   float attenuation = 1 / (light.constant + light.linear * lightDistance + light.quadratic * lightDistance * lightDistance);

   return (diffuse + specular) * attenuation * intensity;
}