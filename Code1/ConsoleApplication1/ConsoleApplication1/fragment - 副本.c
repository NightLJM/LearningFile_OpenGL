#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Material {	//材质
	sampler2D diffuse;
	sampler2D specular;
    float shininess;
}; 
struct Light {		//平行光和点光源
    vec4 posordir;//平行光源的话，方向由光源指向物体
	vec4 lightcolor;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	
	float constant;
    float linear;
    float quadratic;
};
struct SpotLight{	//聚光
	vec4 position;
	vec4 front;
	vec4 lightcolor;
	
    vec4 diffuse;
    vec4 specular;
	
	float constant;
    float linear;
    float quadratic;
	
	float cutoff;
	float outcutoff;
};
uniform vec4 viewPosition;
uniform Material material;
uniform Light light;
uniform SpotLight camera;

vec4 LightCalc(Light light,Material material,vec3 FragPos,vec3 Normal,vec3 viewDir);
vec4 SpotLightCalc(SpotLight light,Material material,vec3 FragPos,vec3 Normal,vec3 viewDir);

void main()
{
	vec3 viewPos=vec3(viewPosition.x,viewPosition.y,viewPosition.z);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec4 clolor1=LightCalc(light,material,FragPos,Normal,viewDir);
	
	vec4 color2=SpotLightCalc(camera,material,FragPos,Normal,viewDir);
	
	vec4 result=color1+color2;
	FragColor=result;
	
}

vec4 LightCalc(Light light,Material material,vec3 FragPos,vec3 Normal,vec3 viewDir)//viewDir须标准化
{
//-----------------------------------环境光----------------------------------
	vec4 ambient=light.ambient*texture(material.diffuse,TexCoord);	
	
//-----------------------------------漫反射----------------------------------
	vec3 lightDir;
	float attenuation=1.0f;
	if(light.posordir.w==1.0f)	//如果是点光源
	{
		vec3 lightPos=vec3(light.posordir.x,light.posordir.y,light.posordir.z);
		lightDir=normalize(lightPos - FragPos);
		float distance = length(lightPos - FragPos);
		attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	}
	else						//如果是平行光
	{	
		vec3 lightdirv3=vec3(light.posordir.x,light.posordir.y,light.posordir.z);
		lightDir=normalize(-lightdirv3);
	}
	vec3 norm=normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff*light.diffuse*texture(material.diffuse, TexCoord);
	
//---------------------------------镜面高光--------------------------------------
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular=light.specular*spec*texture(material.specular,TexCoord);
	
//---------------------------------衰减----------------------------------------
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	
	return ((ambient+diffuse+specular)*light.lightcolor);
}

vec4 SpotLightCalc(SpotLight light,Material material,vec3 FragPos,vec3 Normal,vec3 viewDir)
{
//-----------------------------衰减与边缘柔化系数--------------------------------
	vec3 lightPos=vec3(light.position.x,light.position.y,light.position.z);//位置
	vec3 lightFront=vec3(light.front.x,light.front.y,light.front.z);//正方向
	vec3 lightDir=normalize(lightPos - FragPos);;//方向
	float theta=dot(-lightDir,normalize(lightFront));
	float epsilon   = light.cutoff - light.outcutoff;
	float intensity = clamp((theta - light.outcutoff) / epsilon ,0.0f,1.0f);//柔化系数
	float distance=length(lightPos - FragPos);
	float attenuation=1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));//衰减系数
//------------------------------漫反射-------------------------------------
	vec3 norm=normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff*light.diffuse*texture(material.diffuse, TexCoord);
//------------------------------镜面高光--------------------------------------
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular=light.specular*spec*texture(material.specular,TexCoord);
	
//------------------------------衰减与边缘柔化------------------------------
	diffuse*=attenuation;
	specular*=attenuation;	//衰减
	diffuse*=intensity;
	specular*=intensity;	//柔化
	
	return ((diffuse+specular)*light.lightcolor);
}
