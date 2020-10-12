#version 330 core
out vec4 FragColor;

in vec3 fColor;

void main()
{
    FragColor = vec4(vec3(fColor),1.0f);   
}