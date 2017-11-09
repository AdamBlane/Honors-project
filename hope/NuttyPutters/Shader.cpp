// Externals
#include <iostream>
#include <fstream>
// Internals
#include "Shader.h"


// Created by Calum - 5TH of October - Used to identify and store shader information
// Added to engine 11/10 by Matt 


static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& filename);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

// Constructor
Shader::Shader(const std::string& filename)
{
	// Create a new shader program 
	m_program = glCreateProgram();
	// Allocate and builder shaders
	// Vertex Shader
	m_shaders[0] = CreateShader(LoadShader((filename) + ".vert"), GL_VERTEX_SHADER);
	// Fragment Shader
	m_shaders[1] = CreateShader(LoadShader((filename) + ".frag"), GL_FRAGMENT_SHADER);
	
	// Adds all the shaders to the shader program
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		// Loop through shaders
		glAttachShader(m_program, m_shaders[i]);
	}
	
	// Pass the values into shader, number and name must match values in vert shader
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "tex_coord_in");

	// Link shaders
	glLinkProgram(m_program);
	// As the linking of the shaders and program can be problematic, call error checker
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program failed to link: ");

	// Validate Program
	glValidateProgram(m_program);
	// Once the program is complete - shaders added, built, linked - do one final check to make sure it works
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program failed to validate sucessfully: ");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

// Deconstructor
Shader::~Shader()
{
	// Delete the individual shaders
	// Loop through all the shaders
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		// Detach shader from program then delete shader
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	// Delete shader program
	glDeleteProgram(m_program);
}

// Bind the program
void Shader::Bind()
{
	glUseProgram(m_program);
}

void Shader::Update(const Transform& transform, glm::mat4 camera)
{
	glm::mat4 model = camera * transform.getModel();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, glm::value_ptr(model));
}

// Method which attempts to load shader. If fails then it produces an error
static std::string LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		std::cout << "Shader loaded successfully! " << fileName << "\n" << std::endl;

		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cout << "Unable to load shader! " << fileName << std::endl;
	}

	return output;
}

// As there can be many errors with Shaders and I/O this generates a more efficent error message
static void CheckShaderError(GLuint shader, GLuint flag, bool isPorgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isPorgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE)
	{
		if (isPorgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		std::cout << errorMessage << ": '" << error << ". Sorry!" << std::endl;
	}
}

static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	// If shader equals nothing then error
	if (shader == 0)
	{
		std::cout << "Error: Shader creation failed! " << shaderType << std::endl;
	}

	// List of strings which has the file path 
	//const GLchar* shaderSourceStrings[1];
	//GLint shaderSourceStringLengths[1];

	// Convert the file name to char and get length
	//shaderSourceStrings[0] = text.c_str();
	//shaderSourceStringLengths[0] = text.length();

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	// Set values to shader
	glShaderSource(shader, 1, p, lengths);
	// Compile shader
	glCompileShader(shader);

	// Do another error check incase things have gone wrong again
	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed ");

	return shader;
}
