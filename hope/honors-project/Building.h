#pragma once

#include "glew_glfw.h"
#include "include\glm-0.9.8.5\glm\glm\glm.hpp"
enum type
{
	Skyscraper,
	House,
	Barrier,
	Car
};
class building
{
	public:
		building() {};
		~building() {};
		const glm::vec3& get_Posistion() const { return position; }
		void set_Posistion(const glm::vec3 &value) { position = value; }

		const glm::vec3& get_Size() const { return size; }
		void set_Size(const glm::vec3 &value) { size = value; }

		const glm::vec3& get_Rot() const { return rotation; }
		void set_Rot(const glm::vec3 &value) { rotation = value; }

		void set_Type(const type &value) { type = value; }
		const type& get_Type() const { return type; }

	private:
		type type;
		glm::vec3 position;
		glm::vec3 size;
		glm::vec3 rotation;
	
};

