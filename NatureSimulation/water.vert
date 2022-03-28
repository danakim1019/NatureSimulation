#version 450

layout(location=0) in vec4 vertexPos;
layout(location=1) in vec3 vertexCol;

uniform vec4 plane;

uniform mat4 mvp;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 clipSpace;

out vec3 fcolors;

void main()
{
		vec4 worldPosition = modelMatrix * vertexPos;

		gl_ClipDistance[0]=dot(worldPosition,plane);

		clipSpace = mvp*vertexPos;	

		gl_Position = clipSpace;				//gl_Position : in-build variable for vertex position in NDC

		fcolors = vertexCol;

}