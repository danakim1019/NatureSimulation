#version 450 core

layout (location = 0) in vec3 coord3d;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec3 texCoords;
out vec3 color;

uniform vec4 LightPosition;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 LightIntensity;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 NormalMatrix;

void main()
{
	//change vertex position : 모든 light 계산은 camera좌표계에서 이루어짐
	mat4 ModelViewMatrix = viewMatrix * modelMatrix;
	mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

	vec4 P= ModelViewMatrix * vec4(coord3d,1.0); //camera좌표계에서의 vertex위치값
	//normal 변경
	vec3 N = normalize(NormalMatrix * v_normal); //local 좌표계에서의 normal을 변경
	
	vec3 L = normalize(vec3(LightPosition - P));	//내적계산을 위해 vec3로 변경

	vec3 V = normalize(vec3(-P));			//CameraPositon(0,0,0) - P = -P
	vec3 R = normalize(reflect(-L,N));		//반사

	vec3 ambient = LightIntensity * Ka;
	vec3 diffuse = LightIntensity * Kd * max(dot(L,N),0);
	vec3 spec = LightIntensity * Ks * pow(max(dot(R,V), 0.0), 10.0);

	color = ambient + diffuse + spec;

	//color = LightIntensity * Kd * max(0,dot(L,N));
	
	gl_Position = mvp * vec4(coord3d,1.0);

}
