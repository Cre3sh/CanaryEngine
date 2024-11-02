#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;  // From vertex shader
in vec3 Normal;   // From vertex shader

uniform vec3 ViewPos;  // Position of the viewer/camera
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct Light {
    vec3 LightPosition; // 12 bytes
    vec3 LightColor;    // 12 bytes
    float Intensity;    // 4 bytes
    int LightType;      // 4 bytes
    float LightRadius;  // 4 bytes
    vec3 LightDirection; // 12 bytes
    float LightCutOff;   // 4 bytes
};

uniform int numLights;
uniform Light lights[100];

vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient, diffuse, specular;

    // Determine light direction based on type
    vec3 lightDir;
    if (light.LightType == 0) 
    {
        // Directional light
        lightDir = normalize(-light.LightDirection);
    }
    else 
    {
        // Point light
        lightDir = normalize(light.LightPosition - fragPos);
        // Calculate attenuation based on distance
        float distance = length(light.LightPosition - fragPos);
        float attenuation = 1.0 / (distance * distance); // Simple attenuation
        light.Intensity *= attenuation; // Modify intensity based on distance
    }

    // Ambient lighting
    ambient = 0.1 * light.Intensity * light.LightColor;

    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = diff * light.Intensity * light.LightColor;

    // Specular lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 32.0; // You can pass this as a uniform variable
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    specular = spec * light.Intensity * light.LightColor;

    // Spotlight cutoff (if needed)
    if (light.LightType == 2) 
    {
        float theta = dot(lightDir, normalize(-light.LightDirection));
        if (theta < cos(light.LightCutOff)) return vec3(0.0); // Outside spotlight cone
    }

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(ViewPos - FragPos);

    vec3 result = vec3(0.0);
    for (int i = 0; i < numLights; i++) 
    {
        result += calculateLight(lights[i], normal, FragPos, viewDir);
    }

    // Sample the texture color
    vec4 textureColor = texture(texture_diffuse1, TexCoords);
    if (numLights == 1)
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    else if (numLights <= 0)
    {
        FragColor = vec4(0.5, 0.3, 0.7, 0.3);
    }

    // Combine the texture color with the lighting result
    FragColor = vec4(result, 1.0) * textureColor; // Multiply lighting result with texture color
}