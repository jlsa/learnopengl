#version 330 core

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight {
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform DirectionalLight light;
uniform vec3 viewPos;

uniform float time;

void main()
{
	
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	//vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// specular
	//float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // power of 2
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;//texture(material.specular, TexCoords).rgb;

	// emission
	vec3 emission = vec3(0.0);
	if (texture(material.specular, TexCoords).r == 0.0)
	{
		// apply emission texture
		//emission = texture(material.emission, TexCoords).rgb;

		// some extra fun stuff with "time uniform"
		//emission = texture(material.emission, TexCoords + vec2(0.0, time)).rgb; // moving
		//emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;
	}
	

	// final result
	vec3 result = ambient + diffuse + specular + emission;

	FragColor = vec4(result, 1.0);
}