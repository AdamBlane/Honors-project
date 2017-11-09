#version 440

// Sampler used to get texture colour
uniform sampler2D tex;

// Incoming texture coordinate
layout (location = 0) in vec2 tex_coord;
// Outgoing colour
layout (location = 0) out vec4 out_colour;

void main()
{
	vec4 g = texture(tex, tex_coord);
	
	if (g.a >= 0.3f && g.a < 1.0f)
	{
		g.a = 0.8f;
	}
	else if (g.a < 0.3f)
	{
		discard;
	}
	out_colour = g;

}