#version 460 core
out vec4 FragColor;

in vec2 TexCoords; // You must pass this from the vertex shader
in vec3 Normal;
in vec3 FragPos; 
  
uniform sampler2D diffuseTexture; // Rename to something meaningful
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    //ambient and texture color
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuseTexColor = texture(diffuseTexture, TexCoords).rgb;

    //diffuse and normals
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular 
    float specularStrength = 0.8; //typically 0.5

    vec3 viewDir = normalize(viewPos- FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64); //set 64 to 32
    vec3 specular = specularStrength * spec * lightColor;

    //result
    vec3 result = (ambient + diffuse + specular) * diffuseTexColor;
    FragColor = vec4(result, 1.0);
}