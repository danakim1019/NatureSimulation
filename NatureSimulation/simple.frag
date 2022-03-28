#version 400 //indicating opengl version

in vec4 fcolors;

out vec4 FragColors;

void main()
{
		FragColors = fcolors;
}