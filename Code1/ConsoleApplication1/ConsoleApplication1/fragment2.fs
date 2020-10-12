#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    float shininess;
	sampler2D ourTexture;
	sampler2D specular;
}; 
struct Light {
    vec4 posordir;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	
	float constant;
    float linear;
    float quadratic;
};

struct Camera{
	vec4 position;
	vec4 direction;
	vec4 lightcolor;
	float cutoff;
	float outcutoff;
	
};

uniform Material material;
uniform Light light;
uniform Camera camera;
uniform vec4 ourcolor;
uniform vec4 lightcolor;
void main()
{
	//--------------------------------普通光源--------------------
	vec4 ambient=light.ambient * material.ambient*texture(material.ourTexture, TexCoord);	//环境光

	vec3 lightDir;//漫反射
	float distance;
	float attenuation=1.0f;
	if(light.posordir.w==1.0f)
	{
		vec3 lightPos=vec3(light.posordir.x,light.posordir.y,light.posordir.z);//点光源
		lightDir=normalize(lightPos - FragPos);//点光源
		float distance = length(lightPos - FragPos);
		attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	}
	else
	{
		vec3 lightdirv3=vec3(light.posordir.x,light.posordir.y,light.posordir.z);//平行光源
		lightDir=normalize(-lightdirv3);//平行光源
	}
	vec3 norm=normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * material.diffuse * light.diffuse*texture(material.ourTexture, TexCoord);
	
	
	vec3 viewPos=vec3(camera.position.x,camera.position.y,camera.position.z);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = spec * texture(material.specular, TexCoord) * light.specular;
	
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	
	vec4 result=(ambient+diffuse+specular)*lightcolor;
	
//---------------------------------摄像机光----------------------------	
	vec3 cameraFront=vec3(camera.direction.x,camera.direction.y,camera.direction.z);
	float theta = dot(viewDir, normalize(-cameraFront));
	float epsilon   = camera.cutoff - camera.outcutoff;
	float intensity = clamp((theta - camera.outcutoff) / epsilon ,0.0f,1.0f);
	float cameradis=length(viewPos - FragPos);
	float cameraatt = 1.0 / (light.constant + light.linear * cameradis + light.quadratic * (cameradis * cameradis));
	float cameraspec=pow(max(dot(viewDir, -cameraFront), 0.0), material.shininess);
	vec4 cameraspecular=cameraspec*texture(material.specular, TexCoord)*light.specular;
	result=cameraatt*intensity*(max(dot(norm, viewDir), 0.0)*material.diffuse * light.diffuse*texture(material.ourTexture, TexCoord)+cameraspecular)*camera.lightcolor+result;
	
	FragColor = result*ourcolor;
}
