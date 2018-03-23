#pragma once

#include "glew_glfw.h"
#include "include\glm-0.9.8.5\glm\glm\glm.hpp"

class building
{
	public:
		building();
		~building();
		const glm::vec3& get_Posistion() const { return position; }
		void set_Posistion(const glm::vec3 &value) { position = value; }

		const glm::vec3& get_Size() const { return size; }
		void set_Size(const glm::vec3 &value) { size = value; }

		enum type
		{
			Skyscraper,
			House,
			Barrier
		};

	private:
		glm::vec3 position;
		glm::vec3 size;
	
};
