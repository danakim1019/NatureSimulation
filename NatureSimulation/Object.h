#ifndef OBJECT_H
#define OBJECT_H

#include"Loader.h"

#include<vector>

#include"Transform.h"

namespace simulation {
	class Object : public Transform{
	public:
		std::string objectName;
		ShaderProgram* m_shaderProgram;
		float* vertexPos, vertexColor, vertices;
		GLuint vaoHandle;
		GLuint vbo_vertexPos, vbo_colors;

		const char* getName() {
			const char* name = objectName.c_str();
			return name;
		}
		Object(char name[]) {
			objectName = name;
		}

	};
}

#endif // !OBJECT_H
