#ifndef WATER_H
#define WATER_H

#include <glm/mat4x4.hpp>

#include<iostream>
#include"Loader.h"

#include<vector>

#include<glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<math.h>
#include"Object.h"
#include"ModelView.h"

//Å¸ÀÏ °¹¼ö 60°³
namespace simulation {
	class Water : public Object{
	public:
		Water(char name[],int reflectWidth =0, int reflectHeight =0,int tileSize = 128,glm::vec4 planeVec = glm::vec4(0,-1,0,15));
		~Water();
		void setup();
		void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
	private:
		ModelView m_model;

		ShaderProgram* m_shaderProgram;
		float* vertexPos, vertexColor, vertices;
		GLuint vaoHandle;
		GLuint vbo_vertexPos, vbo_colors;

		//water GLuint
		GLuint reflectionFrameBuffer;
		GLuint reflectionTexture;
		GLuint reflectionDepthBuffer;

		GLuint refractionFrameBuffer;
		GLuint refractionTexture;
		GLuint refractionDepthTexture;

		GLuint reflectTex, refractTex;

		void cleanUp();

		void bindReflectionFrameBuffer();
		void bindRefractionFrameBuffer();

		void unbindCurrentFrameBuffer();

		int getReflectionTexture() { return reflectionTexture; }
		int getRefractionTexture() { return refractionTexture; }
		int getRefractionDepthTexture() { return refractionDepthTexture; }

		void initializeReflectionFrameBuffer();
		void initializeRefractionFrameBuffer();
		void bindFrameBuffer(GLuint frameBuffer, int width, int height);
		int createFrameBuffer();
		int createTextureAttachment(int width, int height);
		int createDepthTextureAttachment(int width, int height);
		int createDepthBufferAttachment(int width, int height);

		int tileSize,tileXY;		//Á¦°ö
		int reflectWidth, reflectHeight;
		int refractWidth, refractHeight;
		
		glm::vec4 planeVec;

	};

}
#endif // !WATER_H
