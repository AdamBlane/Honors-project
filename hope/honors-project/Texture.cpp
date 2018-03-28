#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>


// Consturctor
Texture::Texture(const std::string& fileName)
{
	isCubeMap = false;
	// Store the width, height and number of components that make up a texture
	// NumCoponents is rarely used but good to have incase using complex textures
	int width, height, numComponents;

	// Load the texture data
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cout << "Texture loading failed for texture: " << fileName << std::endl;
	}

	// Generate the texture
	glGenTextures(1, &m_texture);
	// Bind a 2D texture
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Optional - can specify texture wrapping if outside the texture bounds
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// If image takes up more pixels than specifies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Anisotropic filter active
	float max_anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);

	// Input texture information
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	//Generate mipmap of texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free the GPU of the texture data
	stbi_image_free(imageData);
}

Texture::Texture(const std::string& PosXFilename, const std::string& NegXFilename, const std::string& PosYFilename, const std::string& NegYFilename, const std::string& PosZFilename,
	const std::string& NegZFilename)
{
	isCubeMap = true;
	// Store the width, height and number of components that make up a texture
	// NumCoponents is rarely used but good to have incase using complex textures
	int widthPosX, heightPosX, numComponentsPosX;
	int widthNegX, heightNegX, numComponentsNegX;
	int widthPosY, heightPosY, numComponentsPosY;
	int widthNegY, heightNegY, numComponentsNegY;
	int widthPosZ, heightPosZ, numComponentsPosZ;
	int widthNegZ, heightNegZ, numComponentsNegZ;

	// Load the texture data
	unsigned char* PosX = stbi_load(PosXFilename.c_str(), &widthPosX, &heightPosX, &numComponentsPosX, 4);
	unsigned char* NegX = stbi_load(NegXFilename.c_str(), &widthNegX, &heightNegX, &numComponentsNegX, 4);
	unsigned char* PosY = stbi_load(PosYFilename.c_str(), &widthPosY, &heightPosY, &numComponentsPosY, 4);
	unsigned char* NegY = stbi_load(NegYFilename.c_str(), &widthNegY, &heightNegY, &numComponentsNegY, 4);
	unsigned char* PosZ = stbi_load(PosZFilename.c_str(), &widthPosZ, &heightPosZ, &numComponentsPosZ, 4);
	unsigned char* NegZ = stbi_load(NegZFilename.c_str(), &widthNegZ, &heightNegZ, &numComponentsNegZ, 4);

	if (PosX == NULL || NegX == NULL || PosY == NULL || NegY == NULL || PosZ == NULL || NegZ == NULL)
	{
		std::cout << "Texture loading failed for cubemap " << std::endl;
	}
	glEnable(GL_TEXTURE_CUBE_MAP);
	// Generate the texture
	glGenTextures(1, &m_texture);
	// Bind a 2D texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	// Optional - can specify texture wrapping if outside the texture bounds
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	// If image takes up more pixels than specifies
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Anisotropic filter active
	float max_anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);

	// Input texture information
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, widthPosX, heightPosX, 0, GL_RGBA, GL_UNSIGNED_BYTE, PosX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, widthNegX, heightNegX, 0, GL_RGBA, GL_UNSIGNED_BYTE, NegX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, widthPosY, heightPosY, 0, GL_RGBA, GL_UNSIGNED_BYTE, PosY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, widthNegY, heightNegY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NegY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, widthPosZ, heightPosZ, 0, GL_RGBA, GL_UNSIGNED_BYTE, PosZ);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, widthNegZ, heightNegZ, 0, GL_RGBA, GL_UNSIGNED_BYTE, NegZ);

	//Generate mipmap of texture
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Free the GPU of the texture data
	stbi_image_free(PosX);
	stbi_image_free(NegX);
	stbi_image_free(PosY);
	stbi_image_free(NegY);
	stbi_image_free(PosZ);
	stbi_image_free(NegZ);
}


// Deconstructor
Texture::~Texture()
{
	// Delete texture once complete
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind(unsigned int unit)
{
	// Make sure unit does not go out of range - can only be between 0 and 31
	assert(unit >= 0 && unit <= 31);

	// Set active texture to the value stored in the unit variable
	glActiveTexture(GL_TEXTURE0 + unit);
	// Bind that texture to the unit
	if (isCubeMap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
}
