#version 330 core
#define NUM_POINT_LIGHTS 4

out vec4 color;
in vec3 fNormal;
in vec3 fPosition;
in vec2 fTexCoords;

struct Material{
	sampler2D texture_diffuse1;
	vec3 specular;
	float shininess;
};

struct PointLight{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float lin;
	float quad;
};

uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPosition;
uniform int actualPointLights;

vec3 processPointLight(PointLight light, vec3 normal, vec3 fPosition, vec3 viewDirection, vec3 rawTexture){
	vec3 ambient = light.ambient * rawTexture;

	vec3 lightDirection = normalize(light.position - fPosition);
	float diff = max(dot(normal,lightDirection),0.0);
	vec3 diffuse = light.diffuse * diff * rawTexture;

	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	float distance = length(light.position - fPosition);
	float attenuation = 1.0/(light.constant + light.lin*distance + light.quad*(distance*distance));
	return (ambient + diffuse + specular)*attenuation;
}

void main(){
	vec3 normal = normalize(fNormal);
	vec3 viewDirection = normalize(viewPosition - fPosition);
	vec3 rawTexture = vec3(texture(material.texture_diffuse1, fTexCoords));

	vec3 resultingColor = vec3(0.0);
	for(int i = 0; i < actualPointLights; i++){
		resultingColor += processPointLight(pointLights[i], normal, fPosition, viewDirection, rawTexture);
	}



	color = vec4(resultingColor,1.0);
}