#version 330 core

in vec2 texCoords;
in vec3 normal;
in vec3 position;

out vec4 fragColour;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;
uniform sampler2D normalTexture;

uniform bool hasAplha;
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

uniform samplerCube depthTexture;
uniform float farPlane;

float calculateShadow()
{
	vec3 v = position - lightPosition.xyz;
	float depth = texture(depthTexture, v).r * farPlane;

	float currentDepth = length(v);

	return currentDepth - 0.05 > depth ? 1.0 : 0.0;
}

void main()
{
	if (hasAplha && texture(alphaTexture, texCoords).r == 0) {
		discard;
	}

	float distance = length(lightPosition.xyz - position);
	vec3 s = normalize(distance - position);
	vec3 v = normalize(vec3(-position));
	vec3 h = normalize(v + s);

	float attenuation = 5000.0 / (lightCoef.x + lightCoef.y * distance + lightCoef.z * (distance * distance));

	vec3 ka = mix(lightAmbient.xyz, ambient, 0.5) * vec3(texture(ambientTexture, texCoords));
	vec3 kd = lightDiffuse.xyz * diffuse * vec3(texture(diffuseTexture, texCoords)) * max(dot(s, normal), 0.0);
	vec3 ks = lightSpecular.xyz * specular;

	if(hasSpecular) {
		ks *= vec3(texture(specularTexture, texCoords));
	}

	ks *= pow(max(dot(h, normal), 0.0), specularExponent);

	fragColour = vec4(attenuation *  (ka + (1.0 - calculateShadow()) *(kd + ks)), 1.0f);
}