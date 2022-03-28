#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glm/mat4x4.hpp>

#include<iostream>
#include"Loader.h"

#include<vector>

#include<glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<math.h>

namespace simulation {
	class Rectangle {
	public:
		Rectangle();
		void setup();
		void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
	private:
		ShaderProgram* m_shaderProgram;
		float* vertexPos, vertexColor, vertices;
		GLuint vaoHandle;
		GLuint vbo_vertexPos, vbo_colors;
	};

}
#endif // !RECTANGLE_H
