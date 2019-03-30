#version 400 

in vec3 fragNormal;
in vec4 worldPosition;
in vec2 fragTextureCoords;
in float visibility;
in vec4 fragPosLightSpace;
out vec4 fragmentColour;

#define nrL 6

uniform vec3 lightsPosition[nrL];
uniform vec3 lightsColor[nrL];
uniform vec3 lightsAttenuation[nrL];
uniform mat4 model;
uniform vec3 skyColor;
uniform vec3 cameraPosition;

uniform sampler2D blendMap;
uniform sampler2D backgroundTexture;
uniform sampler2D textureR;
uniform sampler2D textureG;
uniform sampler2D textureB;
uniform sampler2D shadowMap;

vec3 color;
float specularStrength = 0.2f; // pot fi puse in uniform pentru fiecare obiect
float shininess = 32.0f;

vec4 textureColor;

float shadow;

void computeLight()
{	
	vec3 totalDiffuse = vec3(0.0f);
	vec3 totalSpecular = vec3(0.0f);

	vec3 norm = normalize(model * vec4(fragNormal, 0.0f)).xyz; // vezi daca nu merge dim vertex shadder
	vec3 unitToCameraVector = normalize(cameraPosition - worldPosition.xyz);
	float attenuation = 0.0f;

	for(int i = 0; i < nrL; i++)
	{
		float distanceLightFrag = length(lightsPosition[i] - worldPosition.xyz);
		attenuation = (lightsAttenuation[i].x + lightsAttenuation[i].y * distanceLightFrag + lightsAttenuation[i].z * distanceLightFrag * distanceLightFrag);
		// difuza
		vec3 toLightDir = normalize( lightsPosition[i] - worldPosition.xyz);
		totalDiffuse += max( dot( norm, toLightDir ), 0.0 ) * lightsColor[i] / attenuation;
		
		// speculara
		vec3 lightDirection = -toLightDir;
		vec3 reflectedLightDirection = reflect(lightDirection, norm);
		vec3 halfWayDir = normalize( toLightDir + unitToCameraVector );
		//float specularFactor = dot(halfWayDir, norm);
		float specularFactor = dot(reflectedLightDirection, unitToCameraVector.xyz);
		specularFactor = max(specularFactor, 0.0f);
		float dampedFactor = pow(specularFactor, shininess);
		
		totalSpecular += (dampedFactor * lightsColor[i]) / attenuation;
	}

	// ambientala
	totalDiffuse = max(totalDiffuse, 0.2f);
	totalSpecular *= specularStrength;
	totalSpecular = max(totalSpecular, 0.0f);

	color = max((1.0f - shadow) * (totalDiffuse + totalSpecular), 0.2f);
}

void getTextureColor()
{
	vec4 blendMapColor = texture(blendMap, fragTextureCoords);
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = fragTextureCoords * 100.0;
	
	vec4 textureBackColor = texture(backgroundTexture, tiledCoords.xy) * backTextureAmount;
	vec4 textureRColor = texture(textureR, tiledCoords) * blendMapColor.r;
	vec4 textureGColor = texture(textureG, tiledCoords) * blendMapColor.g;
	vec4 textureBColor = texture(textureB, tiledCoords) * blendMapColor.b;
	
	textureColor = min(textureBackColor + textureRColor + textureGColor + textureBColor, 1.0f);
}

void computeShadow()
{
	// perspective divide
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	normalizedCoords.y = 1.0f - normalizedCoords.y;
	if (normalizedCoords.z > 1.0f)
	{
		shadow = 0.0f;
		return;
	}
	// transform to [0, 1]
	normalizedCoords = normalizedCoords * 0.5f + 0.5f;
	//normalizedCoords.y = 1.0f - normalizedCoords.y;
	// closest depth value 
	float closestDepth = texture(shadowMap, normalizedCoords.xy).x;
	// current depth
	float currentDepth = normalizedCoords.z;
	// check if in shadow
	float bias = 0.005f;
	shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
}

void computeShadow2()
{
	vec2 ndc = (fragPosLightSpace.xy/fragPosLightSpace.w)/2.0f + 0.5f;
	float objectNearestLight = texture(shadowMap, ndc).r;

	if (fragPosLightSpace.z > objectNearestLight)
	{
		shadow = 1.0f;
	}
	else
	{
		shadow = 0.0f;
	}
}

void main() {

	computeShadow();
	getTextureColor();
	computeLight();

    fragmentColour = vec4(color, 0.0f) * textureColor;
	fragmentColour = mix(vec4(skyColor, 1.0f), fragmentColour, visibility);
}