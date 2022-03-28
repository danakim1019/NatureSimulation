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
	//change vertex position : ��� light ����� camera��ǥ�迡�� �̷����
	mat4 ModelViewMatrix = viewMatrix * modelMatrix;
	mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

	vec4 P= ModelViewMatrix * vec4(coord3d,1.0); //camera��ǥ�迡���� vertex��ġ��
	//normal ����
	vec3 N = normalize(NormalMatrix * v_normal); //local ��ǥ�迡���� normal�� ����
	
	vec3 L = normalize(vec3(LightPosition - P));	//��������� ���� vec3�� ����

	vec3 V = normalize(vec3(-P));			//CameraPositon(0,0,0) - P = -P
	vec3 R = normalize(reflect(-L,N));		//�ݻ�

	vec3 ambient = LightIntensity * Ka;
	vec3 diffuse = LightIntensity * Kd * max(dot(L,N),0);
	vec3 spec = LightIntensity * Ks * pow(max(dot(R,V), 0.0), 10.0);

	color = ambient + diffuse + spec;

	//color = LightIntensity * Kd * max(0,dot(L,N));
	
	gl_Position = mvp * vec4(coord3d,1.0);

}
