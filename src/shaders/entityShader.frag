#version 400

in vec3 fragNormal;
in vec4 worldPosition;
in vec2 fragTextureCoords;
in float visibility;
out vec4 fragmentColour;

#define nrL 6

uniform vec3 lightsPosition[nrL];
uniform vec3 lightsColor[nrL];
uniform vec3 lightsAttenuation[nrL];
uniform	mat4 model;
uniform vec3 skyColor;
uniform vec3 cameraPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

vec3 color;
float specularStrength = 0.5f;
float shininess = 32.0f;

void computeLight()
{
	vec3 totalDiffuse = vec3(0.0f);
	vec3 totalSpecular = vec3(0.0f);

	vec3 norm = normalize(model * vec4(fragNormal, 0.0f)).xyz;
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
		float specularFactor = dot(halfWayDir, norm);
		//float specularFactor = dot(reflectedLightDirection, unitToCameraVector.xyz);
		specularFactor = max(specularFactor, 0.0f);
		float dampedFactor = pow(specularFactor, shininess);
		
		totalSpecular += specularStrength * dampedFactor * lightsColor[i] / attenuation;
	}

	// ambientala
	totalDiffuse = max(totalDiffuse, 0.2f);
	
	totalDiffuse *= vec3(texture( diffuseTexture, fragTextureCoords )); 
	totalSpecular = vec3(texture( specularTexture, fragTextureCoords ));

	color = min((totalDiffuse + totalSpecular), 1.0f);
}

void main() {

	computeLight();

    fragmentColour = vec4(color, 1.0);
	fragmentColour = mix(vec4(skyColor, 1.0f), fragmentColour, visibility); // visibility = 0 -> sky color , = 1 -> vertex color;
}