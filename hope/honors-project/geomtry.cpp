
#include "geometry.h"

/*
Creates a new geometry object
*/
geometry::geometry() throw (...) :
	_type(GL_TRIANGLES),
	_vao(0),
	_index_buffer(0),
	_vertices(0),
	_indices(0)
{
}

// Move constructor
geometry::geometry(geometry &&other)
{
	_type = other._type;
	_vao = other._vao;
	_buffers = std::move(other._buffers);
	_index_buffer = other._index_buffer;
	_vertices = other._vertices;
	_indices = other._indices;
	_minimal = other._minimal;
	_maximal = other._maximal;
	other._buffers = std::map<GLuint, GLuint>();
}

// Adds a buffer to the geometry object
bool geometry::add_buffer(const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type)
{
	// Check that index is viable
	assert(index < 16);
	// Check that buffer is not empty
	assert(buffer.size() > 0);
	// Check if geometry initialised
	if (_vao == 0)
	{
		// Create the vertex array object
		glGenVertexArrays(1, &_vao);
		// Check for any OpenGL error
	}
	// If we have no vertices yet, set the vertices to the size of this buffer
	if (_vertices == 0)
		_vertices = buffer.size();
	// Otherwise ensure that the number of vertices matches
	else if (_vertices != buffer.size())
	{
		std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
		std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
		return false;
	}
	// Now add buffer to the vertex array object.  Bind the vertex array object
	glBindVertexArray(_vao);
	// Generate buffer with OpenGL
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	// Set the buffer data
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec2), &buffer[0], buffer_type);
	// Set the vertex pointer and enable
	glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);
	// Check for OpenGL error
	// Add buffer to map
	_buffers[index] = id;
	return true;
}

// Adds a buffer to the geometry object
bool geometry::add_buffer(const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type)
{
	// Check that index is viable
	assert(index < 16);
	// Check that buffer is not empty
	assert(buffer.size() > 0);
	// Check if geometry initialised
	if (_vao == 0)
	{
		// Create the vertex array object
		glGenVertexArrays(1, &_vao);
		// Check for any OpenGL error
	}
	// If we have no vertices yet, set the vertices to the size of this buffer
	if (_vertices == 0)
		_vertices = buffer.size();
	// Otherwise ensure that the number of vertices matches
	else if (_vertices != buffer.size())
	{
		std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
		std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
		return false;
	}
	// Now add buffer to the vertex array object.  Bind the vertex array object
	glBindVertexArray(_vao);
	// Generate buffer with OpenGL
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	// Set the buffer data
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec3), &buffer[0], buffer_type);
	// Set the vertex pointer and enable
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);
	// Check for OpenGL error
	// Add buffer to map
	_buffers[index] = id;
	return true;
}

// Adds a buffer to the geometry object
bool geometry::add_buffer(const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type)
{
	// Check that index is viable
	assert(index < 16);
	// Check that buffer is not empty
	assert(buffer.size() > 0);
	// Check if geometry initialised
	if (_vao == 0)
	{
		// Create the vertex array object
		glGenVertexArrays(1, &_vao);
		// Check for any OpenGL error
	}
	// If we have no vertices yet, set the vertices to the size of this buffer
	if (_vertices == 0)
		_vertices = buffer.size();
	// Otherwise ensure that the number of vertices matches
	else if (_vertices != buffer.size())
	{
		std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
		std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
		return false;
	}
	// Now add buffer to the vertex array object.  Bind the vertex array object
	glBindVertexArray(_vao);
	// Generate buffer with OpenGL
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	// Set the buffer data
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec4), &buffer[0], buffer_type);
	// Set the vertex pointer and enable
	glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(index);
	// Check for OpenGL error
	// Add buffer to map
	_buffers[index] = id;
	return true;
}

// Adds an index buffer to the geometry
bool geometry::add_index_buffer(const std::vector<GLuint> &buffer)
{
	// Check that buffer is not empty
	assert(buffer.size() > 0);
	// Check if vertex array object is valid
	assert(_vao != 0);
	// Set indices to buffer size
	_indices = buffer.size();
	// Bind vertex array object
	glBindVertexArray(_vao);
	// Add buffer
	glGenBuffers(1, &_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(GLuint), &buffer[0], GL_STATIC_DRAW);
	// Check for error
	return true;
}
