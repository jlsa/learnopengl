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

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform PointLight light;
uniform PointLight light2;
uniform vec3 viewPos;

uniform float time;

void main()
{
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(-light.direction);
	vec3 lightDir = normalize(light.position - FragPos);
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
	
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// ------------
	// light 2
	// ------------

	// ambient
	vec3 ambient2 = light2.ambient * texture(material.diffuse, TexCoords).rgb;

	// diffuse
	vec3 norm2 = normalize(Normal);
	//vec3 lightDir = normalize(-light.direction);
	vec3 lightDir2 = normalize(light2.position - FragPos);
	float diff2 = max(dot(norm2, lightDir2), 0.0);
	vec3 diffuse2 = light2.diffuse * diff2 * texture(material.diffuse, TexCoords).rgb;

	// specular
	//float specularStrength = 0.5;
	vec3 viewDir2 = normalize(viewPos - FragPos);
	vec3 reflectDir2 = reflect(-lightDir2, norm2);

	float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), material.shininess); // power of 2
	vec3 specular2 = light2.specular * spec2 * texture(material.specular, TexCoords).rgb;//texture(material.specular, TexCoords).rgb;

	// emission
	vec3 emission2 = vec3(0.0);
	if (texture(material.specular, TexCoords).r == 0.0)
	{
		// apply emission texture
		//emission = texture(material.emission, TexCoords).rgb;

		// some extra fun stuff with "time uniform"
		//emission = texture(material.emission, TexCoords + vec2(0.0, time)).rgb; // moving
		//emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;
	}
	
	float distance2 = length(light2.position - FragPos);
	float attenuation2 = 1.0 / (light2.constant + light2.linear * distance2 + light2.quadratic * (distance2 * distance2));

	ambient2 *= attenuation2;
	diffuse2 *= attenuation2;
	specular2 *= attenuation2;



	// final result
	vec3 result = ambient + ambient2 + diffuse + diffuse2 + specular + specular2 + emission + emission2;

	FragColor = vec4(result, 1.0);
}