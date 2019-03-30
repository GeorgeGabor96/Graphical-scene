#version 400

in vec3 fragNormal;
in vec4 fragPosEye;
in vec2 fragTextureCoords;
in float visibility;
out vec4 fragmentColour;

uniform vec3 lightsPosition[4];
uniform vec3 lightsColor[4];
uniform vec3 lightsAttenuation[4];
uniform vec3 skyColor;

uniform sampler2D blendMap;
uniform sampler2D backgroundTexture;
uniform sampler2D textureR;
uniform sampler2D textureG;
uniform sampler2D textureB;

vec3 color;
float ambientStrength = 0.6f;
float specularStrength = 0.2f;
float shininess = 32.0f;

vec4 textureColor;

void computeLight()
{	
	vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalAmbient = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalDiffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 totalSpecular = vec3(0.0f, 0.0f, 0.0f);
	vec3 toLightDir = vec3(0.0f, 0.0f, 0.0f);
	
	vec3 norm = normalize(fragNormal); // vezi daca nu merge dim vertex shadder
	vec3 toCameraDir = normalize( cameraPosition - fragPosEye.xyz );

	totalAmbient += ambientStrength * lightsColor[0];


	for(int i = 0; i < 4; i++)
	{
		float distanceLightFrag = length(lightsPosition[i] - fragPosEye.xyz);
		float attenuation = 1.0f / (lightsAttenuation[i].x + lightsAttenuation[i].y * distanceLightFrag + lightsAttenuation[i].z * distanceLightFrag * distanceLightFrag);
		// ambient
		// diffuse
		toLightDir = normalize( lightsPosition[i] - fragPosEye.xyz );
		totalDiffuse += max( dot( norm, toLightDir ), 0.0 ) * lightsColor[i] * attenuation;
		// speculara
		vec3 halfWayDir = normalize( toLightDir + toCameraDir );
		totalSpecular += specularStrength * pow( max( dot( norm, halfWayDir), 0.0 ), shininess ) * lightsColor[i] / attenuation;
	}

	color = min(((totalAmbient + totalDiffuse + totalSpecular)), 1.0f);
}

void getTextureColor()
{
	vec4 blendMapColor = texture(blendMap, fragTextureCoords);
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = fragTextureCoords * 100.0;

	vec4 textureBackColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 textureRColor = texture(textureR, tiledCoords) * blendMapColor.r;
	vec4 textureGColor = texture(textureG, tiledCoords) * blendMapColor.g;
	vec4 textureBColor = texture(textureB, tiledCoords) * blendMapColor.b;
	
	textureColor = min(textureBackColor + textureRColor + textureGColor + textureBColor, 1.0f);
}

void main() {
	getTextureColor();
	computeLight();

    fragmentColour = textureColor * vec4(color,0.0f);
	fragmentColour = mix(vec4(skyColor, 1.0f), fragmentColour, visibility);
}