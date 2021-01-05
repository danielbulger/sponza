#version 330 core

in vec3 fragPos;
in vec3 normals;
in vec2 texCoords;

out vec4 fragColor;

uniform vec3 lightPosition;
uniform vec3 lightIntensity;
uniform float lightAttenuation;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;

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

    vec3 n = normalize(normals);
    vec3 s = normalize(lightPosition - fragPos);
    vec3 v = normalize(vec3(-fragPos));
    vec3 h = normalize(v + s);

    vec3 ka = ambient * vec3(texture(ambientTexture, texCoords)) * lightIntensity;
    vec3 kd = diffuse * vec3(texture(diffuseTexture, texCoords)) * max(dot(s, normals), 0.0) * lightIntensity;
    vec3 ks = specular * vec3(texture(specularTexture, texCoords)) * pow(
        max(dot(h, n), 0.0), specularExponent
    ) * lightIntensity;

    vec3 color = ka + kd + ks;

    fragColor = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
}