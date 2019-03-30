#version 140

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

out vec4 outColor;

uniform sampler2D particleTexture;

void main()
{
	vec4 color1 = texture(particleTexture, textureCoords1);
	vec4 color2 = texture(particleTexture, textureCoords2);
	outColor = mix(color1, color2, blend);
}