#pragma once
// Externals
#include <string>
#include "glew_glfw.h"
// Internals
#include "Transform.h"
#include "Camera.h"
// GLFW includes


// Created by Calum - 5TH of October - Used to identify and store shader information
// Added to engine 11/10 by Matt

class Shader
{
public:
	Shader(const std::string& fileName);
	// Bind the shader method
	void Bind();
	void Update(const Transform& transform, glm::mat4 VP);

	virtual ~Shader();
private:
	// Num_shaders is used to identify the type - frag and vertex. If we need geometry shader change to 3.
	static const unsigned int NUM_SHADERS = 2;
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {}

	enum
	{
		TRANSFORM_U,
		NUM_UNIFORMS
	};

	// Handle which keeps track of the program and will be used to keep track of the shader
	GLuint m_program;

	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
};

