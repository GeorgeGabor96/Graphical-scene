#version 400

in vec3 fragNormal;
in vec4 fragPosEye;
in vec2 fragTextureCoords;
in float visibility;
out vec4 fragmentColour;

uniform vec3 lightsPosition[4];
uniform vec3 lightsColor[4];
uniform vec3 cameraPosition;
uniform	mat3 normalMatrix;
uniform vec3 skyColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

vec3 color;
float ambientStrength = 0.2f;
float specularStrength = 0.5f;
float shininess = 32.0f;

float constant = 1.0f;
float linear = 0.0035f;
float quadratic = 5.0f;//0.0075f;

void computeLight()
{
	vec3 cameraPosEye = vec3(0.0f);
	vec3 totalAmbient = vec3(0.0f);
	vec3 totalDiffuse = vec3(0.0f);
	vec3 totalSpecular = vec3(0.0f);

	vec3 norm = normalize(normalMatrix * fragNormal); // vezi daca nu merge dim vertex shadder
	vec3 toCameraDir = normalize(cameraPosEye - fragPosEye.xyz);
	
	for(int i = 0; i < 4; i++)
	{
		vec3 toLightDir = normalize( lightsPosition[i] - fragPosEye.xyz );
		float distanceLightFrag = length(lightsPosition[i] - fragPosEye.xyz);
		float atenuare = 1.0f / (constant + linear * distanceLightFrag + quadratic * distanceLightFrag * distanceLightFrag);

		// ambient
		totalAmbient += ambientStrength * lightsColor[i];
		// diffuse
		totalDiffuse += max(dot(norm, toLightDir), 0.0) * lightsColor[i];

		// speculara
		vec3 halfWayDir = normalize( toLightDir + toCameraDir );
		vec3 specular = specularStrength * pow( max( dot( norm, halfWayDir), 0.0 ), shininess ) * lightsColor[i];
	}

	totalAmbient *= vec3(texture(diffuseTexture, fragTextureCoords));
	totalDiffuse *= vec3(texture( diffuseTexture, fragTextureCoords )); 
	totalSpecular *= vec3(texture( specularTexture, fragTextureCoords ));

	color = min((totalDiffuse), 1.0f);
}

void main() {

	computeLight();

    fragmentColour = vec4(color, 1.0);
	fragmentColour = mix(vec4(skyColor, 1.0f), fragmentColour, visibility); // visibility = 0 -> sky color , = 1 -> vertex color

}