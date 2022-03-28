#version 450

layout(location=0) in vec4 vertexPos;
layout(location=1) in vec3 vertexCol;

uniform mat4 mvp;

out vec4 fcolors;

void main()
{
		gl_Position = mvp*vertexPos;					//gl_Position : in-build variable for vertex position in NDC

		fcolors = vec4(vertexCol,1.0);

}