/**
* @file MyGlWindow.h
* @date 2022-02-02
* @author danakim1019(gh87913711@gmail.com)
* @brief glfwWindow
*/
#ifndef _MYGLWINDOW_
#define _MYGLWINDOW_

#include <iostream>
#include <string>

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_inverse.hpp"

#include"ModelView.h"

#include"Rectangle.h"
#include"Water.h"
#include"grid.h"
#include"Model.h"

#include"Camera.h"

namespace simulation {
	class MyGlWindow {
	public:
		MyGlWindow(int w, int h);
		~MyGlWindow();
		ShaderProgram* m_shaderProgram;

		void setSize(int width, int height);
		void setAspect(float ratio);
		void draw(camera cam, float zoom);

		GLuint vaoHandle;			//
		GLuint vbo_vetexPositon, vbo_colors;

		Rectangle* rect;
		Water* water;
		Model* loadedModel;

	private:
		ModelView m_model;

		grid* m_grid;

		int m_width;
		int m_height;
		int first;
		float m_ratio;
		void setupBuffer();
	};
}


#endif // !_MYGLWINDOW_