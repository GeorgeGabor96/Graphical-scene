#version 400 core

layout(location = 0) in vec3 vPosition;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform vec3 lightPos;

float tiling = 4.0f;

// pentru ceata
const float density = 0.0035f;
const float gradient = 5.0f;

void main()
{
	vec4 worldPosition = model * vec4(vPosition, 1.0f);
	clipSpace = projection * view * worldPosition;
	gl_Position = clipSpace;

	textureCoords = vec2(vPosition.x / 2.0f + 0.5f, vPosition.z / 2.0f + 0.5f) * tiling;
	toCameraVector = cameraPos - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPos;

	float distanceFromCamera = length(view * model * vec4(vPosition, 1.0f)); 
	visibility = exp(-pow((distanceFromCamera * density), gradient));
	visibility = clamp(visibility, 0.0f, 1.0f);
}
