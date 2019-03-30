#version 410 core

in vec3 textureCoordinates;
out vec4 color;

uniform samplerCube skyboxDay;
uniform samplerCube skyboxNight;
uniform vec3 fogColor;
uniform float blendFactor;

const float lowerLimit = 0.0f;
const float upperLimit = 0.3f; // skybox e definit in coordonate normalizate

void main()
{
    vec4 textureColorDay = texture(skyboxDay, textureCoordinates);
    vec4 textureColorNight = texture(skyboxNight, textureCoordinates);

    vec4 textureColor = mix(textureColorDay, textureColorNight, blendFactor);

    float factor = (textureCoordinates.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0f, 1.0f);

    color = mix(vec4(fogColor, 1.0f), textureColor, factor); 
}
