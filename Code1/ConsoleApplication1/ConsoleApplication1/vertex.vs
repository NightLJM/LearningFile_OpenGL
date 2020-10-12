#version 330 core
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec2 aColor;
layout (location = 4) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    gl_Position = projection*view*model*vec4(aPos, 1.0);
    TexCoord = aColor;
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	
}