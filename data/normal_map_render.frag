#version 330 core

in vec2 texCoords;
in vec3 lightDirection;
in vec3 viewDirection;

out vec4 fragColor;

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

    // Map into -1 to 1 range.
    vec3 normal = (2.0 * texture(normalTexture, texCoords, -1.0) - 1.0).rgb;

    float dotN = max(dot(normal, lightDirection), 0.0);
    vec3 reflectedLightDirection = reflect(-lightDirection, normal);
    float specularFactor = max(dot(reflectedLightDirection, viewDirection), 0.0);

    vec3 ka = ambient * vec3(texture(ambientTexture, texCoords));
    vec3 kd = diffuse * vec3(texture(diffuseTexture, texCoords)) * dotN;
    vec3 ks = specular * vec3(texture(specularTexture, texCoords)) * pow(specularFactor, specularExponent);

    vec3 color = lightIntensity * (ka + kd + ks);

    fragColor = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
}