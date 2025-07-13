#version 330 core

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
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

void main() {
   vec3 norm = normalize(Normal);
   vec3 lightDirection = normalize(LightPos - FragPos);

   // ambient
   vec3 ambient = light.ambient * material.ambient;

   // diffuse
   float diffuseStrength = max(0.0, dot(lightDirection, norm));
   vec3 diffuse = light.diffuse * material.diffuse * diffuseStrength;

   // specular
   vec3 viewDirection = normalize(-FragPos);
   vec3 reflectedDirection = reflect(-lightDirection, norm);
   float specularStrengh = pow(max(dot(viewDirection, reflectedDirection), 0), material.shininess);
   vec3 specular = light.specular * material.specular * specularStrengh;

   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);
}