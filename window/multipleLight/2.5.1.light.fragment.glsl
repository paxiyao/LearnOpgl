#version 330 core
out vec4 FragColor;

in vec3 AFragColor;

void main()
{
    FragColor = vec4(AFragColor, 1.0);
}