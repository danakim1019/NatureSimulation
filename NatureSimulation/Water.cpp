#include"Water.h"

int screenWidth;
int screenHeight;

namespace simulation {

	Water::Water(char name[], int reflectWidth, int reflectHeight,int tileSize,glm::vec4 planeVec):Object(name){
		this->reflectWidth = reflectWidth;
		refractWidth = this->reflectWidth * 6;
		this->reflectHeight = reflectHeight;
		refractHeight = this->refractHeight * 6;
		this->tileSize = tileSize;
		tileXY = glm::sqrt(tileSize);
		this->planeVec = planeVec;
		setup();
		//reflect, refract load
		initializeReflectionFrameBuffer();
		initializeRefractionFrameBuffer();
	}

	/// <summary>
	/// 소멸자
	/// </summary>
	Water::~Water() {
		cleanUp();
	}

	void Water::setup() {

		float vertices[] = {
			0.0f,1.0f,0.0f,1.0f, 	0,0,0.5f,
			0,0.0f,0.0f,1.0f,	    0,0,0.5f,
			1.0f,0,0.0f,1.0f,	   0,0,0.5f,
			1.0f,0,0,1,              0,0,0.5f,
			1.0f,1.0f,0,1,          0,0,0.5f,
			0,1.0f,0,1,              0,0,0.5f,
		};

		//shader object
		m_shaderProgram = new ShaderProgram();
		m_shaderProgram->initFromFiles("water.vert", "water.frag");

		// create VAO(vertex array object)
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		//add attributes and uniform vars
		m_shaderProgram->addUniform("mvp");

		m_shaderProgram->addUniform("modelMatrix");
		m_shaderProgram->addUniform("viewMatrix");
		m_shaderProgram->addUniform("projectionMatrix");

		m_shaderProgram->addUniform("plane");

		m_shaderProgram->addUniform("reflectionTexture");
		m_shaderProgram->addUniform("refractionTexture");

		glGenBuffers(1, &vbo_vertexPos);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexPos);
		//allocate a memory on GPU side..
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//how to interpret the data
		glVertexAttribPointer(
			0,	//id numberf : property number
			4,	//number of data for each vertex	(homogeneous coord)
			GL_FLOAT,		//data type
			GL_FALSE,		//is it normalized?
			sizeof(float) * 7,		//how many data for each vertex (x,y,z,w,r,g,b)
			0	//the gap from the beginnig
		);
		glEnableVertexAttribArray(0);				//id number와 같아야함

		glGenBuffers(1, &vbo_colors);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
		//allocate a memory on GPU side..
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//how to interpret the data
		glVertexAttribPointer(
			1,	//id numberf : property number
			3,	//number of data for each vertex	(homogeneous coord)
			GL_FLOAT,		//data type
			GL_FALSE,		//is it normalized?
			sizeof(float) * 7,
			(void*)(sizeof(float) * 4)			//r,g,b data after x,y,z,w
		);
		glEnableVertexAttribArray(1);				//id number와 같아야함

		glBindVertexArray(0);
	}

	void Water::cleanUp() {
		glDeleteFramebuffers(1,&reflectionFrameBuffer);
		glDeleteTextures(1,&reflectionTexture);
		glDeleteRenderbuffers(1,&reflectionDepthBuffer);
		glDeleteFramebuffers(1, &refractionFrameBuffer);
		glDeleteTextures(1, &refractionTexture);
		glDeleteTextures(1, &refractionDepthTexture);
	}

	/// <summary>
	///  call before rendering to this FBO
	/// </summary>
	void Water::bindReflectionFrameBuffer() {
		bindFrameBuffer(reflectionFrameBuffer,reflectWidth,reflectHeight);
	}

	/// <summary>
	/// call before rendering to this FBO
	/// </summary>
	void Water::bindRefractionFrameBuffer() {
		bindFrameBuffer(refractionFrameBuffer,refractWidth,refractHeight);
	}

	/// <summary>
	/// 
	/// </summary>
	void Water::unbindCurrentFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
	}

	/// <summary>
	/// 
	/// </summary>
	void Water::initializeReflectionFrameBuffer() {
		reflectionFrameBuffer = createFrameBuffer();
		reflectionTexture = createTextureAttachment(reflectWidth,reflectHeight);
		reflectionDepthBuffer = createDepthBufferAttachment(reflectWidth,reflectHeight);
		unbindCurrentFrameBuffer();
	}

	/// <summary>
	/// 
	/// </summary>
	void Water::initializeRefractionFrameBuffer() {
		refractionFrameBuffer = createFrameBuffer();
		refractionTexture = createTextureAttachment(refractWidth, refractHeight);
		refractionDepthTexture = createDepthTextureAttachment(refractWidth,refractHeight);
		unbindCurrentFrameBuffer();
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="frameBuffer"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void Water::bindFrameBuffer(GLuint frameBuffer, int width, int height) {
		glGenTextures(1,&frameBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
		glViewport(0, 0, width, height);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int Water::createFrameBuffer() {
		GLuint frameBuffer;
		glGenFramebuffers(1,&frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		return frameBuffer;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	int Water::createTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,
			0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,texture,0);
		return texture;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	int Water::createDepthTextureAttachment(int width, int height) {
		GLuint depthTexture;
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D,depthTexture);
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,width,height,
			0, GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depthTexture, 0);
		return depthTexture;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	int Water::createDepthBufferAttachment(int width, int height) {
		GLuint depthBuffer;
		glGenRenderbuffers(1,&depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER,depthBuffer);
		glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		return depthBuffer;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="model"></param>
	/// <param name="view"></param>
	/// <param name="projection"></param>
	void Water::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {

		//model matrix를 60개 for문 계산
		for (int i = 0; i < tileXY; i++) {
			for (int j = 0; j < tileXY; j++) {
				m_model.glPushMatrix();
				m_model.glTranslate(i, j, 5.0f);

				glm::mat4 mview = view * m_model.getMatrix();
				glm::mat4 mvp = projection * view * m_model.getMatrix();
				/*glm::mat4 mview = view * model;
				glm::mat4 mvp = projection * view * model;*/

				
				m_shaderProgram->use();

				glUniformMatrix4fv(m_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

				glUniformMatrix4fv(m_shaderProgram->uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_model.getMatrix()));
				glUniformMatrix4fv(m_shaderProgram->uniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(m_shaderProgram->uniform("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));

				glUniform4fv(m_shaderProgram->uniform("plane"),1, glm::value_ptr(planeVec));

				glUniform1i(m_shaderProgram->uniform("reflectionTexture"), 0);
				glUniform1i(m_shaderProgram->uniform("refractionTexture"), 1);

				m_model.glPopMatrix();

				//render reflection texture
				//모델뷰를 변경하면됨


				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, reflectionTexture);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, refractionTexture);

				glDisable(GL_CLIP_DISTANCE0);

				glBindVertexArray(vaoHandle);			//pick a VAO and bind it
				glDrawArrays(GL_TRIANGLES, 0, 6);

				m_shaderProgram->disable();

			}
		}
	}
}