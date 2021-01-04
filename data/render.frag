#version 330 core

in vec3 normals;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float specular_exponent;

void main()
{
    vec3 ambient = ambient * vec3(texture(ambientTexture, texCoords));
    vec3 diffuse = diffuse * vec3(texture(diffuseTexture, texCoords));
    vec3 specular = specular * vec3(texture(specularTexture, texCoords));

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}