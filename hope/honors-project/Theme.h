#pragma once
#include "glew_glfw.h"
#include "include\glm-0.9.8.5\glm\glm\glm.hpp"
#include "Building.h"
#include <list>
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
		~theme();
		void create_Buildings(Type type);
		const std::list <building>& get_Building() const { return Buildings; }

		void set_Type(const Type &value) { thing = value; }
		const Type& get_Type() const { return thing; }

	private:
		std::list <building> Buildings;
		Type thing;
};
