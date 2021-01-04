#version 330 core

in vec3 fragPos;
in vec3 normals;
in vec2 texCoords;

out vec4 fragColor;

uniform vec3 lightPosition;
uniform vec3 lightIntensity;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float specularExponent;

void main()
{
    vec3 n = normalize(normals);
    vec3 s = normalize(lightPosition - fragPos);
    vec3 v = normalize(vec3(-fragPos));
    vec3 h = normalize(v + s);

    vec3 ka = ambient * vec3(texture(ambientTexture, texCoords));
    vec3 kd = diffuse * vec3(texture(diffuseTexture, texCoords)) * max(dot(s, normals), 0.0);
    vec3 ks = specular * vec3(texture(specularTexture, texCoords)) * pow(
        max(dot(h, n), 0.0), specularExponent
    );

    float toon = floor(max(0.0, dot(s, normals)) * 3.0) * (1.0 / 3.0);

    fragColor = vec4(lightIntensity * (ka + kd + ks), 1.0);
}