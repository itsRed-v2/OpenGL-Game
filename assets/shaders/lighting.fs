#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

void main() {
   vec3 norm = normalize(Normal);
   vec3 lightDirection = normalize(LightPos - FragPos);

   // ambient
   float ambientStrength = 0.1;
   vec3 ambient = lightColor * ambientStrength;

   // diffuse
   float diffuseStrength = 0.75;
   diffuseStrength *= max(0.0, dot(lightDirection, norm));
   vec3 diffuse = lightColor * diffuseStrength;

   // specular
   float specularStrengh = 0.5;
   vec3 viewDirection = normalize(-FragPos);
   vec3 reflectedLight = reflect(-lightDirection, norm);
   specularStrengh *= pow(max(dot(viewDirection, reflectedLight), 0), 32);
   vec3 specular = lightColor * specularStrengh;

   vec3 result = (ambient + diffuse + specular) * objectColor;
   FragColor = vec4(result, 1.0);
}