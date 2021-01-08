#version 330 core

struct Light {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 coef;
};

layout (std140) uniform LightBlock
{
	Light lights[4];
};


in vec2 texCoords;
in vec3 normal;
in vec3 position;

out vec4 fragColour;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;
uniform sampler2D normalTexture;

uniform bool alpha;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float specularExponent;

vec3 calculateLight(Light light)
{
	float distance = length(light.position.xyz - position);
	vec3 s = normalize(distance - position);
	vec3 v = normalize(vec3(-position));
	vec3 h = normalize(v + s);

	float attenuation = 1000.0 / (light.coef.x + light.coef.y * distance + light.coef.z * (distance * distance));

	vec3 ka = mix(light.ambient.xyz, ambient, 0.5) * vec3(texture(ambientTexture, texCoords));

	vec3 kd = light.diffuse.xyz * diffuse * vec3(texture(diffuseTexture, texCoords)) * max(dot(s, normal), 0.0);

	vec3 ks = light.specular.xyz * specular * vec3(
		texture(specularTexture, texCoords)
	) * pow(max(dot(h, normal), 0.0), specularExponent);

	return attenuation * (ka + (kd + ks));
}

void main()
{

	if (alpha && texture(alphaTexture, texCoords).r == 0) {
		discard;
	}

	vec3 colour = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < 1; ++i) {
		colour += calculateLight(lights[i]);
	}

	fragColour = vec4(colour, 1.0);
}