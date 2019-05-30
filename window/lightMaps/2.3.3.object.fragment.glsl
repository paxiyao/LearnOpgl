#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
uniform Light light;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	// ambient
	vec3 ambient = lightColor * (light.ambient * vec3(texture(material.diffuse, TexCoords)));

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = lightColor * (diff * light.diffuse * vec3(texture(material.diffuse, TexCoords)));

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = lightColor * (spec * light.specular * (vec3(1.0) - vec3(texture(material.specular, TexCoords)))); // here we inverse the sampled specular color. Black becomes white and white becomes black.

	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}