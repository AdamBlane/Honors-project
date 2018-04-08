#define _USE_MATH_DEFINES
#include "Mesh.h"
#include <vector>
#include <math.h>
#include <iostream>

// Create mesh from given obj file
Mesh::Mesh(const std::string& fileName)
{
	IndexedModel model = OBJModel(fileName).ToIndexedModel();
	InitMesh(model);
}

// Create mesh of a certain shape
Mesh::Mesh(typeShape shape, glm::vec3 newPosition, GLfloat size1, GLfloat size2, GLfloat size3, bool isFloor, bool isFluid)
{
	//setting starting geometry properties
	thisShape = shape;
	position = newPosition;
	side1 = size1;
	side2 = size2;
	side3 = size3;
	SetHalfSides();
	isThisFloor = isFloor;
	isThisFluid = isFluid;

	//calling method to create desired shape
	chooseGeometry();
}
Mesh::Mesh(typeShape shape, glm::vec3 newPosition, GLfloat size1, std::vector<glm::vec3> positions, std::vector<glm::vec2> tex_coords, GLfloat size2 , GLfloat size3 , bool isFloor , bool isFluid) 
{
	//setting starting geometry properties
	thisShape = shape;
	position = newPosition;
	side1 = size1;
	side2 = size2;
	side3 = size3;
	SetHalfSides();
	isThisFloor = isFloor;
	isThisFluid = isFluid;
	terrain(positions, tex_coords);

}
// Skybox constructor
// TODO - needs editing such that it does not create a new texture, 
// instead it takes an already initialised texture(s) as param
Mesh::Mesh(Texture* tex)
{
	//	skyTex = new Texture(filenames[0], filenames[1], filenames[2], filenames[3], filenames[4], filenames[5]);
	skyTex = *tex;
	thisShape = SKYBOX;
	chooseGeometry();
}

// Returns position of mesh geometry
glm::vec3 Mesh::GetGeomPos()
{
	return position;
}

// Convert degrees to radians
double Mesh::DegtoRads(double degreesAngle)
{
	double rads = (degreesAngle * M_PI) / 180.0;
	return rads;
}

// Set the scale of this mesh
void Mesh::SetScale(GLfloat size1, GLfloat size2, GLfloat size3)
{
	side1 = size1;
	side2 = size2;
	side3 = size3;
	SetHalfSides();
	// Then redraw vertices for this typeshape
	chooseGeometry();
}

void Mesh::SetPos(glm::vec3 pos)
{
	position = pos;
	// Now redraw
	chooseGeometry();
}

void Mesh::SetAsFloor(bool isFloor)
{
	isThisFloor = isFloor;
	chooseGeometry();
}
void Mesh::SetAsFluid(bool isFluid)
{
	isThisFluid = isFluid;
	chooseGeometry();
}
// Deconstructor
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

//method to create geometry from chosen type
void Mesh::chooseGeometry()
{
	switch (thisShape)
	{
	case TRIANGLE:
		triangle();
		break;
	case QUAD:
		quad();
		break;
	case RECTANGLE:
		rectangle();
		break;
	case PLANE:
		plane();
		break;
	case BOX:
		box();
		break;
	case CUBOID:
		cuboid();
		break;
	case SKYBOX:
		skyBox();
		break;
	default:
		cuboid();
		break;
	}
} 
void Mesh::triangle()
{
	//setting vertices and texture coodrinates for the shape 
	//vertices are created in a counter-clock wise order, to face camera
	Vertex vertices[] = 
	{
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z), glm::vec2(0.0, 0.0)), //index 0
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z), glm::vec2(1.0, 0.0)), //index 1
		Vertex(glm::vec3(position.x, position.y + halfSide1, position.z), glm::vec2(0.5, 1.0)), //index 2
	};

	//setting indices to create the shape
	unsigned int indices[] = {
		0, 1, 2
	};

	//calling generate mesh method, taking array of vertices and indices and how many they are
	generateMesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}
void Mesh::quad()
{
	Vertex vertices[] = {
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z), glm::vec2(1.0, 1.0)), //index 0
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z), glm::vec2(0.0, 1.0)), //index 1
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z), glm::vec2(1.0, 0.0)), //index 2
																											//TR2
																											///get index 2
																											///get index 1
																											Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z), glm::vec2(0.0, 0.0)), //index 3
	};

	unsigned int indices[] = {
		0, 1, 2, 2, 1, 3
	};

	generateMesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

}
void Mesh::rectangle()
{
	Vertex vertices[] = {
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z), glm::vec2(0.0, 1.0)), //index 0
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z), glm::vec2(1.0, 1.0)), //index 1
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z), glm::vec2(0.0, 0.0)), //index 2
																											//TR2
																											///get index 2
																											Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z), glm::vec2(0.0, 0.0)), //index 2
																																																				///get index 1
																																																				Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z), glm::vec2(1.0, 1.0)), //index 1
																																																				Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z), glm::vec2(1.0, 0.0)), //index 3
	};

	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5
	};

	generateMesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}

void Mesh::terrain(std::vector<glm::vec3> positions, std::vector<glm::vec2> tex_coords)
{
	const int a = 1000;
	Vertex vertecies[a] = {};
	unsigned int indexs[a] = {};
	for (unsigned int i = 0; i <a; i++)
	{
		unsigned int temp_index = 0;
		vertecies[i] = Vertex(positions.at(i), tex_coords.at(i));
		indexs[i] = temp_index;
		temp_index++;

	}
	generateMesh(vertecies, sizeof(vertecies) / sizeof(vertecies[0]), indexs, sizeof(indexs) / sizeof(indexs[0]));
}

void Mesh::plane()
{
	Vertex vertices[] = {
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y, position.z + halfSide1), glm::vec2(0.0, 0.0)), //index 0
		Vertex(glm::vec3(position.x + halfSide1, position.y, position.z + halfSide1), glm::vec2(1.0, 0.0)), //index 1
		Vertex(glm::vec3(position.x - halfSide1, position.y, position.z - halfSide1), glm::vec2(0.0, 1.0)), //index 2
																											//TR2
																											///get index 1
																											Vertex(glm::vec3(position.x + halfSide1, position.y, position.z - halfSide1), glm::vec2(1.0, 1.0)), //index 3
																																																				///get index 2
	};

	unsigned int indices[] = {
		0, 1, 2, 1, 3, 2
	};

	generateMesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}
void Mesh::box()
{
	//Counter-clock wise

	Vertex vertices[] = {
		/////////FRONT
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z + halfSide1), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z + halfSide1), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z + halfSide1), glm::vec2(1.0, 1.0)),

		/////////BACK
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z - halfSide1), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z - halfSide1), glm::vec2(1.0, 0.0)),
		//TR2
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z - halfSide1), glm::vec2(1.0, 0.0)),

		/////////LEFT
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z - halfSide1), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z + halfSide1), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),

		/////////RIGHT
		//TR1
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z - halfSide1), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		//TR2
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z + halfSide1), glm::vec2(1.0, 1.0)),

		/////////TOP
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z + halfSide1), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),

		/////////BOTTOM
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		//TR2
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z + halfSide1), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide1, position.z - halfSide1), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide1, position.z - halfSide1), glm::vec2(1.0, 1.0))
	};

	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};

	generateMesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

}

void Mesh::cuboid()
{
	//Counter-clock wise

	Vertex vertices[] = {
		/////////FRONT
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z + halfSide3), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z + halfSide3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z + halfSide3), glm::vec2(1.0, 1.0)),

		/////////BACK
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z - halfSide3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z - halfSide3), glm::vec2(1.0, 0.0)),
		//TR2
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z - halfSide3), glm::vec2(1.0, 0.0)),

		/////////LEFT
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z - halfSide3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z + halfSide3), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),

		/////////RIGHT
		//TR1
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z - halfSide3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		//TR2
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z + halfSide3), glm::vec2(1.0, 1.0)),

		/////////TOP
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z + halfSide3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y + halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),

		/////////BOTTOM
		//TR1
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		//TR2
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z + halfSide3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - halfSide1, position.y - halfSide2, position.z - halfSide3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + halfSide1, position.y - halfSide2, position.z - halfSide3), glm::vec2(1.0, 1.0))
	};

	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};

	generateMesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}

void Mesh::skyBox()
{
	//Counter-clock wise

	Vertex vertices[] = {
		/////////FRONT
		//TR1
		Vertex(glm::vec3(position.x - skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z + skySize3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),

		//TR2
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z + skySize3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y + skySize2, position.z + skySize3), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),


		/////////BACK
		//TR1
		Vertex(glm::vec3(position.x - skySize1, position.y - skySize2, position.z - skySize3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z - skySize3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z - skySize3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(1.0, 1.0)),

		/////////LEFT
		//TR1
		Vertex(glm::vec3(position.x - skySize1, position.y - skySize2, position.z - skySize3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),
		//TR2
		Vertex(glm::vec3(position.x - skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z + skySize3), glm::vec2(1.0, 1.0)),

		/////////RIGHT
		//TR1
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z - skySize3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y + skySize2, position.z + skySize3), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),

		/////////TOP
		//TR1
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z + skySize3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y + skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),
		//TR2
		Vertex(glm::vec3(position.x + skySize1, position.y + skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y + skySize2, position.z - skySize3), glm::vec2(1.0, 1.0)),

		/////////BOTTOM
		//TR1
		Vertex(glm::vec3(position.x - skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y - skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
		//TR2
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z + skySize3), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(position.x + skySize1, position.y - skySize2, position.z - skySize3), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(position.x - skySize1, position.y - skySize2, position.z - skySize3), glm::vec2(0.0, 1.0)),
	};

	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};

	generateMesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
}

void Mesh::generateMesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}

	for (unsigned int i = 0; i < numIndices; i++)
	{
		model.indices.push_back(indices[i]);
	}

	InitMesh(model);

}

void Mesh::generateMeshs(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}

	for (unsigned int i = 0; i < numIndices; i++)
	{
		model.indices.push_back(indices[i]);
	}

	InitMesh(model);
}


void Mesh::InitMesh(const IndexedModel& model)
{
	m_drawCount = model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// Create required buffers
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	// Set Position values
	// Take this buffer in as an array
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	// Put the data on the GPU - size of vertex * number of vertices
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);
	// Tell OpenGL what to do with the position data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Set Tex values
	// Take this buffer in as an array
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	// Put the data on the GPU - size of vertex * number of vertices
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);
	// Tell OpenGL what to do with the position data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Set Index values
	// Take this buffer in as an array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	// Put the data on the GPU - size of vertex * number of vertices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}