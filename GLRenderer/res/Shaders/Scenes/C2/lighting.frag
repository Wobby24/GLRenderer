#version 460 core
out vec4 FragColor;

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

in vec2 TexCoords; // You must pass this from the vertex shader
in vec3 Normal;
in vec3 FragPos; 

uniform Material material;
uniform Light light;  
uniform sampler2D diffuseTexture; // Rename to something meaningful
uniform vec3 viewPos;

void main()
{
    //ambient and texture color
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuseTexColor = texture(diffuseTexture, TexCoords).rgb;

    //diffuse and normals
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;

    //specular 
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    //result
    vec3 result = (ambient + diffuse + specular) * diffuseTexColor;
    FragColor = vec4(result, 1.0);
}