#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform bool useEmissionMap;
uniform bool useDiffuseMap;
uniform bool useSpecularMap;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    // === Texture fallback logic ===
    vec3 diffuseTexture = useDiffuseMap
        ? vec3(texture(material.diffuse, TexCoords)).rgb
        : vec3(1.0);  // fallback to white

    vec3 specularTexture = useSpecularMap
        ? vec3(texture(material.specular, TexCoords)).rgb
        : vec3(1.0);  // fallback to white

    vec3 emission = useEmissionMap
        ? texture(material.emission, TexCoords).rgb 
        : vec3(0.0);  // fallback to black (no emission)

    // === Lighting calculations ===
    vec3 ambient = light.ambient * diffuseTexture;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseTexture;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularTexture;

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}
