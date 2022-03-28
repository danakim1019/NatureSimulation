#ifndef TRANSFORM_H
#define TRANSFORM_H

#include<glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace simulation {

	class Transform {
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 sclae;

		void setPsotion(float x,float y,float z) {
			position = glm::vec3(x, y, z);
		}

		glm::vec3 getPosition() {
			return position;
		}

		Transform() {}
	};
}

#endif