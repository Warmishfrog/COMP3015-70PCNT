#version 460

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float Size2;
uniform mat4 ProjectionMatrix;

out vec3 Position; // Output position
out vec3 Normal;   // Output normal
out vec2 TexCoord;


void main()
{
	mat4 m = ProjectionMatrix;

	gl_Position = m * (vec4(-Size2, -Size2, 0.0, 0.0)) +  gl_in[0].gl_Position ; //bottom left
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();
	gl_Position = m * (vec4(Size2, -Size2, 0.0, 0.0)) +  gl_in[0].gl_Position ; //bottom right
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();
	gl_Position = m * (vec4(-Size2, Size2, 0.0, 0.0)) +  gl_in[0].gl_Position ; //top left
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();
	gl_Position = m * (vec4(Size2, Size2, 0.0, 0.0)) +  gl_in[0].gl_Position ; //top right
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();


}