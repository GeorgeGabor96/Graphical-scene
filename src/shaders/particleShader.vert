#version 400

layout(location = 0) in vec2 vPosition;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

uniform vec2 texOffset1;
uniform vec2 texOffset2;
uniform vec2 texCoordInfo; // x - numberRows, y - blend

void main()
{
	vec2 textureCoords = vPosition + vec2(0.5, 0.5);
	//textureCoords.y = 1.0f - textureCoords.y;
	textureCoords /= texCoordInfo.x;
	textureCoords1 = textureCoords + texOffset2;
	textureCoords2 = textureCoords + texOffset1;
	blend = texCoordInfo.y;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(vPosition, 0.0, 1.0);
}