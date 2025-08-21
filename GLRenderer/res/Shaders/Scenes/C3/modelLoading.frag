#version 460 core
out vec4 FragColor;

#define MAX_DIR_LIGHTS 16
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16
#define MAX_TEXTURES 8

struct Material {
    sampler2D diffuse[MAX_TEXTURES];
    sampler2D specular[MAX_TEXTURES];
    sampler2D emission[MAX_TEXTURES];
    float shininess;
    float emissionIntensity;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};  

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

uniform int numDirLights;
uniform int numSpotLights;
uniform int numPointLights;

uniform int numDiffuseTextures;
uniform int numSpecularTextures;
uniform int numEmissionTextures;

uniform DirLight dirLights[MAX_DIR_LIGHTS]; 
uniform SpotLight spotLights[MAX_SPOT_LIGHTS]; 
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform bool useEmissionMap;
uniform bool useDiffuseMap;
uniform bool useSpecularMap;

uniform Material material;
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);

void main()
{
    vec3 diffuseColor = vec3(1.0);
    vec3 specularColor = vec3(1.0);
    vec3 emission = vec3(0.0);

    // Blend active diffuse maps
    if (useDiffuseMap && numDiffuseTextures > 0) {
        diffuseColor = vec3(0.0);
        for (int i = 0; i < MAX_TEXTURES; ++i) {
            if (i >= numDiffuseTextures) break;
            diffuseColor += texture(material.diffuse[i], TexCoords).rgb;
        }
        diffuseColor /= float(numDiffuseTextures);
    }

    // Blend active specular maps
 //   if (useSpecularMap && numSpecularTextures > 0) {
    //    specularColor = vec3(0.0);
     //   for (int i = 0; i < MAX_TEXTURES; ++i) {
      //      if (i >= numSpecularTextures) break;
       //     specularColor += texture(material.specular[i], TexCoords).rgb;
      //  }
       // specularColor /= float(numSpecularTextures);
   // }

   if (useSpecularMap && numSpecularTextures > 0) {
    specularColor = texture(material.specular[0], TexCoords).rrr; // only red channel replicated across RGB
} else {
    specularColor = vec3(1.0);
}


    // Blend active emission maps
    if (useEmissionMap && numEmissionTextures > 0) {
        emission = vec3(0.0);
        for (int i = 0; i < MAX_TEXTURES; ++i) {
            if (i >= numEmissionTextures) break;
            emission += texture(material.emission[i], TexCoords).rgb;
        }
        emission = (emission / float(numEmissionTextures)) * material.emissionIntensity;
    }

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    for (int i = 0; i < MAX_DIR_LIGHTS; ++i) {
        if (i >= numDirLights) break;
        result += CalcDirLight(dirLights[i], norm, viewDir, diffuseColor, specularColor);
    }

    for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        if (i >= numPointLights) break;
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, diffuseColor, specularColor);
    }

    for (int i = 0; i < MAX_SPOT_LIGHTS; ++i) {
        if (i >= numSpotLights) break;
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir, diffuseColor, specularColor);
    }
    emission = vec3(0.0);

    result += emission;


    FragColor = vec4(result, 1.0);

   //FragColor = vec4(1.0, 0.0, 1.0, 1.0); // bright pink for visibility
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularColor;

    vec3 ambient = light.ambient * diffuseColor;

    return (ambient + diffuse + specular) * light.intensity;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.intensity;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;

    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular) * light.intensity;
}
