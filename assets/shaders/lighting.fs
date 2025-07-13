#version 330 core

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

struct Light {
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

out vec4 FragColor;

uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
in vec2 TexCoords;

void main() {
   vec3 norm = normalize(Normal);
   vec3 lightDirection = normalize(LightPos - FragPos);

   // ambient
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

   // diffuse
   float diffuseStrength = max(0.0, dot(lightDirection, norm));
   vec3 diffuse = diffuseStrength * light.diffuse * vec3(texture(material.diffuse, TexCoords));

   // specular
   vec3 viewDirection = normalize(-FragPos);
   vec3 reflectedDirection = reflect(-lightDirection, norm);
   float specularStrengh = pow(max(dot(viewDirection, reflectedDirection), 0), material.shininess);
   vec3 specular = specularStrengh * light.specular * vec3(texture(material.specular, TexCoords));

   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);
}