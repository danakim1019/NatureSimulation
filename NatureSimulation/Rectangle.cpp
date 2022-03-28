#include"Rectangle.h"

namespace simulation {
	Rectangle::Rectangle() {
		setup();
	}

	void Rectangle::setup() {

		float vertices[] = {
			0.0f,1.0f,0.0f,1.0f, 	1,0,0,
			0,0.0f,0.0f,1.0f,	    1,0,0,
			1.0f,0,0.0f,1.0f,	    1,0,0,
			1.0f,0,0,1,              1,0,0,
			1.0f,1.0f,0,1,          1,0,0,
			0,1.0f,0,1,              1,0,0,
		};

		//shader object
		m_shaderProgram = new ShaderProgram();
		m_shaderProgram->initFromFiles("simple.vert", "simple.frag");

		// create VAO(vertex array object)
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		//add attributes and uniform vars
		m_shaderProgram->addUniform("mvp");

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

	void Rectangle::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
		glm::mat4 mview = view * model;
		glm::mat4 mvp = projection * view * model;

		m_shaderProgram->use();

		glUniformMatrix4fv(m_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		glBindVertexArray(vaoHandle);			//pick a VAO and bind it
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_shaderProgram->disable();
	}
}