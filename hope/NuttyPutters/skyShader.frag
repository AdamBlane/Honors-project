#version 440

// Sampler used to get texture colour
uniform samplerCube cubeMap;

// Incoming texture coordinate
layout (location = 0) in vec3 tex_coord;
// Outgoing colour
layout (location = 0) out vec4 out_colour;

void main()
{
	vec4 g = texture(cubeMap, tex_coord);
	out_colour = g;
}