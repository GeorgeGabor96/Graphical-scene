#version 400

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTextureCoords;

out vec4 worldPosition;
out vec3 fragNormal;
//out vec3 toCameraVector;
out vec2 fragTextureCoords;
out float visibility;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 lightSpaceTrMatrix;

uniform	mat3 normalMatrix;
uniform vec4 plane;

// pentru ceata
const float density = 0.0035f;
const float gradient = 5.0f;

void main() {
	worldPosition = model * vec4(vPosition, 1.0f);

    gl_ClipDistance[0] = dot(worldPosition, plane); // distance from vertex to plane

    vec4 positionRelativeToCam = view * worldPosition;
    gl_Position = projection * positionRelativeToCam;
	fragTextureCoords = vTextureCoords;

	fragNormal = vNormal;
	//fragNormal = (model * vec4(vNormal, 0.0)).xyz;

	//toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	fragPosLightSpace = lightSpaceTrMatrix * worldPosition;

	float distanceFromCamera = length(positionRelativeToCam.xyz); 
	visibility = exp(-pow((distanceFromCamera * density), gradient));
	visibility = clamp(visibility, 0.0f, 1.0f);
}