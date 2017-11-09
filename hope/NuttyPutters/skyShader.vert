#version 440

layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 tex_coord_in;

layout (location = 0) out vec3 tex_coord_out;

uniform mat4 transform;

void main()
{
	// Calculate screen position of vertex
	gl_Position = transform * vec4(position, 1.0);
	// Output texture coordinate to fragement shader
	tex_coord_out = position;
}