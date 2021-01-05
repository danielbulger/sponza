#version 330 core

in vec2 texCoords;
in vec3 normal;
in vec3 position;

out vec4 fragColor;

uniform vec3 lightPosition;
uniform vec3 lightIntensity;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;
uniform sampler2D normalTexture;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float specularExponent;

const float gamma = 2.2;

void main()
{

    vec4 alpha = texture(alphaTexture, texCoords);

    if (alpha == vec4(0.0, 0.0, 0.0, 1.0)) {
        discard;
    }

    vec3 s = normalize(vec3(lightPosition) - position);
    vec3 v = normalize(vec3(-position));
    vec3 h = normalize(v + s);

    vec3 ka = ambient * vec3(texture(ambientTexture, texCoords));
    vec3 kd = diffuse * vec3(texture(diffuseTexture, texCoords)) * max(dot(s, normal), 0.0);
    vec3 ks = specular * vec3(texture(specularTexture, texCoords)) * pow(max(dot(h, normal), 0.0), specularExponent);

    vec3 color = lightIntensity * (ka + kd + ks);

    fragColor = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
}