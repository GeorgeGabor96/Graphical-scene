#version 400

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTextureCoords;

out vec3 fragNormal;
out vec4 worldPosition;
out vec2 fragTextureCoords;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 plane;
uniform	mat3 normalMatrix;

// pentru ceata
const float density = 0.0035f;
const float gradient = 5.0f;

void main() {
	fragTextureCoords = vTextureCoords;
	worldPosition = model * vec4(vPosition, 1.0f);
    gl_Position = projection * view * worldPosition;

    gl_ClipDistance[0] = dot(worldPosition, plane); // distance from vertex to plane

	fragNormal = vNormal;
	float distanceFromCamera = length(view * worldPosition); 
	visibility = exp(-pow((distanceFromCamera * density), gradient));
	visibility = clamp(visibility, 0.0f, 1.0f);

	//toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}