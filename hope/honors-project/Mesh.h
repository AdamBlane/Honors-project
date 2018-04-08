#pragma once

#include "glew_glfw.h"
#include "include\glm-0.9.8.5\glm\glm\glm.hpp"
#include "obj_loader.h"
#include "Texture.h"
#include <map>
#include "geometry.h"
#include <vector>
// Class which hold Vertex Information
class Vertex
{
public:
	Vertex() {};
	//Vertex class to hold Vertices positions and Texture coordinates
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
	}

	// Getters
	inline glm::vec3* GetPos() { return &pos; }
	inline glm::vec2* GetTexCoord() { return &texCoord; }
private:
	glm::vec3 pos;
	glm::vec2 texCoord;
};

// Class which holds Mesh Information
class Mesh
{
public:
	//Enum to hold different types of geometry that could be builded
	enum typeShape
	{
		TRIANGLE,
		QUAD,
		RECTANGLE,
		PLANE,
		BOX,
		CUBOID,
		SKYBOX,
		Terrain
	};


	void generateMeshs(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndice);

	//Constructor to load an OBJ file from its file path
	Mesh(const std::string& fileName);

	//Constructor to generate a shape using geometry, taking type of shape, position and dimensions:
	//TRIANGLE: only 1 side is necessary
	//QUAD: only 1 side is necessary
	//RECTANGLE: only 2 sides are necessary
	//PLANE: only 1 side is necessary
	//BOX: only 1 side is necessary
	//CUBOID: all 3 sides are necessary
	Mesh(typeShape shape, glm::vec3 newPosition, GLfloat size1, GLfloat size2 = 1.0f, GLfloat size3 = 1.0f, bool isFloor = false, bool isFluid = false);

	Mesh(typeShape shape, glm::vec3 newPosition, GLfloat size1, std::vector<glm::vec3> positions, std::vector<glm::vec2> tex_coords,GLfloat size2 = 1.0f, GLfloat size3 = 1.0f, bool isFloor = false, bool isFluid = false);
	// Skybox constructor - this needs to be changed (see .cpp)
	Mesh(Texture* tex);

	//Get position of mesh geometry created by geometry builder
	glm::vec3 GetGeomPos();

	//converts degrees to radians
	static double DegtoRads(double degreesAngle);

	//Texture for this Mesh
	Texture thisTexture;

	// Skybox texture
	Texture skyTex;

	// Setter for mesh texture takes in a pointer to texture
	void SetTexture(Texture* tex) { thisTexture = *tex; }

	// Setter for mesh scale
	void SetScale(GLfloat size1, GLfloat size2 = 1.0f, GLfloat size3 = 1.0f);

	// Setter for mesh position
	void SetPos(glm::vec3 pos);

	void SetAsFloor(bool isFloor);
	void SetAsFluid(bool isFluid);

	bool isThisFloor = false;
	bool isThisFluid = false;

	void InitMesh(const IndexedModel& model);

	void Draw();

	virtual ~Mesh();
	// Creates a mesh object with the provided geometry

private:

	void generateMesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	typeShape thisShape = CUBOID;

	
	void chooseGeometry();

	void triangle();
	void quad();
	void rectangle();
	void plane();
	void box();
	void cuboid();
	void skyBox();
	void terrain(std::vector<glm::vec3> positions, std::vector<glm::vec2> tex_coords);
	
	//starting position of geometry
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	GLfloat skySize1 = 500.0f;
	GLfloat skySize2 = 500.0f;
	GLfloat skySize3 = 500.0f;

	//starting size of geometry
	GLfloat side1 = 1.0f;
	GLfloat side2 = 3.0f;
	GLfloat side3 = 1.0f;
	// Vertices creation only used half values of side data
	GLfloat halfSide1;
	GLfloat halfSide2;
	GLfloat halfSide3;
	// Setter for half sides
	void SetHalfSides() {
		halfSide1 = side1 / 2.0f;
		halfSide2 = side2 / 2.0f;
		halfSide3 = side3 / 2.0f;
	}

	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	// The geometry object of the mesh
	geometry geometry;
	// The minimal of the AABB defining the mesh
	glm::vec3 minimal;
	// The maximal of the AABB defining the mesh
	glm::vec3 maximal;

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;

};