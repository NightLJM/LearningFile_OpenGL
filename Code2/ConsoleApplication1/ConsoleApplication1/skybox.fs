#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
	//原
	//FragColor = texture(skybox, TexCoords);

	//gl_FragCoord应用
	if(gl_FragCoord.x < 540)
		FragColor = vec4(vec3(1-texture(skybox, TexCoords)),1);
	else
		FragColor = texture(skybox, TexCoords);
}