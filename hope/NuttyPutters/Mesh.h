#pragma once

#include "glew_glfw.h"
#include "include\glm-0.9.8.5\glm\glm\glm.hpp"
#include "obj_loader.h"
#include "Texture.h"

// Class which hold Vertex Information
class Vertex
{
public:
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
		GOLF_HOLE_GROUND

	};
	Mesh() {};

	//General constructor -> not used at the moment
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

	//Constructor to load a OBJ file from its file path
	Mesh(const std::string& fileName);

	//Constructor to generate a shape using geometry, taking type of shape, file path for texture, position and dimensions:
	//TRIANGLE: only 1 side is necessary
	//QUAD: only 1 side is necessary
	//RECTANGLE: only 2 sides are necessary
	//PLANE: only 1 side is necessary
	//BOX: only 1 side is necessary
	//CUBOID: all 3 sides are necessary
	Mesh(typeShape shape, std::string fileTexture, glm::vec3 newPosition, GLfloat size1, GLfloat size2 = 1.0f, GLfloat size3 = 1.0f, bool isFloor = false, bool isFluid = false);


	Mesh(const std::vector<std::string> &filenames);

	//Get position of geometry created by geometry builder
	glm::vec3 getGeomPos();

	//converts degrees to radians
	static double toRads(double degreesAngle);

	//Texture for each Mesh
	Texture *thisTexture;

	std::string filename;

	bool isThisFloor = false;
	bool isThisFluid = false;

	void InitMesh(const IndexedModel& model);

	void Draw();

	virtual ~Mesh();


private:
	//Mesh(const Mesh& other);
	void operator=(const Mesh& other);

	typeShape thisShape = CUBOID;



	void chooseGeometry();

	void triangle();
	void quad();
	void rectangle();
	void plane();
	void box();
	void cuboid();
	void skyBox();

	//this is broken. DON'T USE IT!!!!
	void golfHole();


	void generateMesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

	//starting position of geometry
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	GLfloat skySize1 = 500.0f;
	GLfloat skySize2 = 500.0f;
	GLfloat skySize3 = 500.0f;

	//starting size of geometry
	GLfloat side1 = 1.0f;
	GLfloat side2 = 3.0f;
	GLfloat side3 = 1.0f;
	GLfloat halfSide1 = 0.5f;
	GLfloat halfSide2 = 0.5f;
	GLfloat halfSide3 = 0.5f;
	GLfloat thisHole = 1.4;

	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;

};