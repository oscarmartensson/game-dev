#version 130

// Input data
in vec2 vertexPosition;
in vec4 vertexColor;

// Output color
out vec2 position;
out vec4 color;

// ----------------------------------
// Main
void main()
{
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	position = vertexPosition;
	color = vertexColor;
}