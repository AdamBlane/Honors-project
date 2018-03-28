#pragma once
#include "glew_glfw.h"
#include "include\glm-0.9.8.5\glm\glm\glm.hpp"
#include "Building.h"
#include <vector>
enum Type
{
	Desert,
	City,
	Village,
	Seaside
};
class theme
{
	public:
		theme();
		~theme() {};
		void create_Buildings(Type type, int coordx, int coordy);
		std::vector <building*> Buildings;
		void set_Type(const Type &value) { thing = value; }
		const Type& get_Type() const { return thing; }

	private:
		Type thing;
};
