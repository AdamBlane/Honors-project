#pragma once

#include "glew_glfw.h"
#include "include\glm-0.9.8.5\glm\glm\glm.hpp"
enum size
{
	Large, Small
};
class health
{
public:
	health() {};
	~health() {};
	const glm::vec3& get_Posistion() const { return position; }
	void set_Posistion(const glm::vec3 &value) { position = value; }

	void set_Size(const size &value) { size = value; }
	const size& get_Size() const { return size; }

	void set_Build(const int &value) { in_building = value; }
	const int& get_Build() const { return in_building; }

private:
	size size;
	glm::vec3 position;
	int in_building;
};

