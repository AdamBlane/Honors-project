#pragma once

#include <string>
#include "glew_glfw.h"

class Texture
{
public:
	Texture(const std::string& fileName);
	Texture(const std::string& PosXFilename, const std::string& NegXFilename, const std::string& PosYFilename, const std::string& NegYFilename, const std::string& PosZFilename, const std::string& NegZFilename);
	// Unit allows you to select a texture
	void Bind(unsigned int unit);

	virtual ~Texture();
private:
	Texture(const Texture& other) {}
	Texture& operator=(const Texture& other) {}
	bool isCubeMap = false;
	GLuint m_texture;
};