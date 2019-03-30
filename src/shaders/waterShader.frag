#version 400 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;
in float visibility;
out vec4 outColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform float moveFactor;
uniform vec3 lightColor;
uniform vec3 skyColor;

const float waveStrength = 0.04f;
const float shineDamper = 20.0f;
const float reflectivity = 0.5f;
const float near = 0.1f;
const float far = 1000.0f;

float conversion(float depth)
{
	return 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
}

void main()
{
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0f + 0.5f; // screen space coordinates
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	
	// find water depth
	float depth = texture(depthMap, refractTexCoords).r;
	float floorDistance = conversion(depth);

	depth = gl_FragCoord.z;
	float waterDistance = conversion(depth);

	float waterDepth = floorDistance - waterDistance;

	vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg*0.1;
	distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+moveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth/20.0f, 0.0f, 1.0f);

	refractTexCoords += totalDistortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
	
	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);


	vec4 normalMapColor = texture(normalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0f -1.0f, normalMapColor.b * 3.0f, normalMapColor.g * 2.0f - 1.0f);
	normal = normalize(normal);

	// fresnel Effect
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor, 2.0f);
	refractiveFactor = clamp(refractiveFactor, 0.0f, 1.0f);



	// specular light
	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth/5.0f, 0.0f, 1.0f);

	outColor = mix(reflectColor, refractColor, refractiveFactor);
	outColor = mix(outColor, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f);
	outColor += vec4(specularHighlights, 0.0f);

	outColor.a = clamp(waterDepth/5.0f, 0.0f, 1.0f);

	outColor = mix(vec4(skyColor, 1.0f), outColor, visibility);

	//outColor = texture(reflectionTexture, reflectTexCoords);


	//outColor = vec4(waterDepth/50.0f); // arata adancimea
}