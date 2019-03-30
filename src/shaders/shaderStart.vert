#version 400

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTextureCoords;

out vec3 fragNormal;
out vec4 fragPosEye;
out vec2 fragTextureCoords;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform	mat3 normalMatrix;

// pentru ceata
const float density = 0.007;
const float gradient = 1.5;

void main() {
    fragPosEye = view * model * vec4(vPosition, 1.0f);
	fragNormal = vNormal;//normalize(normalMatrix * vNormal);;
	fragTextureCoords = vTextureCoords;
    gl_Position = projection * fragPosEye;

	float distanceFromCamera = length(fragPosEye); // camera e la 0 0 0
	visibility = exp(-pow((distanceFromCamera * density), gradient));
	visibility = clamp(visibility, 0.0f, 1.0f);
}