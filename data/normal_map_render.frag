#version 330 core

in vec2 texCoords;
in vec3 viewDirection;
in vec3 worldPosition;
in vec3 lightDirection;
in vec3 lightEyePosition;

out vec4 fragColour;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;
uniform sampler2D normalTexture;

uniform samplerCube depthTexture;
uniform float farPlane;

uniform bool hasAlpha;
uniform bool hasSpecular;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float specularExponent;

uniform vec4 lightPosition;
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 lightCoef;

float calculateShadow()
{
	vec3 v = worldPosition - lightPosition.xyz;
	float depth = texture(depthTexture, v).r * farPlane;

	float currentDepth = length(v);

	return currentDepth - 0.0005 > depth ? 0.0 : 1.0;

}

void main()
{
	if (hasAlpha && texture(alphaTexture, texCoords).r == 0) {
		discard;
	}

	// Map into -1 to 1 range.
	vec3 normal = (2.0 * texture(normalTexture, texCoords, -1.0) - 1.0).rgb;

	vec3 colour = vec3(0.0, 0.0, 0.0);

	float distance = length(lightPosition.xyz - worldPosition);
	float attenuation = 5000 / (lightCoef.x + lightCoef.y * distance + lightCoef.z * (distance * distance));

	float dotN = max(dot(normal, lightDirection), 0.0);
	vec3 reflectedLightDirection = reflect(-lightDirection, normal);
	float specularFactor = max(dot(lightDirection, viewDirection), 0.0);

	vec3 ka = lightAmbient.xyz * vec3(texture(ambientTexture, texCoords));

	if(ambient != vec3(0.0, 0.0, 0.0)) {
		ka *= ambient;
	}

	vec3 kd = lightDiffuse.xyz * diffuse * vec3(texture(diffuseTexture, texCoords)) * dotN;
	vec3 ks = lightSpecular.xyz * specular;

	if (hasSpecular) {
		ks *= vec3(texture(specularTexture, texCoords));
	}

	ks *= pow(specularFactor, specularExponent);

	fragColour = vec4(attenuation * (ka + calculateShadow() * (kd + ks)), 1.0);
}