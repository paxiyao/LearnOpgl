#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

void main()
{
	// ambient
	vec3 ambient = lightColor * (light.ambient * material.ambient);

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = lightColor * (diff * light.diffuse * material.diffuse);

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = lightColor * (spec * light.specular * material.specular);

	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}