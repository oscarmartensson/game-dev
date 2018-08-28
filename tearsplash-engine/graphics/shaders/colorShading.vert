#version 130

// Input data
in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

// Output color
out vec2 position;
out vec4 color;
out vec2 uv;

// ----------------------------------
// Main
void main()
{
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	position = vertexPosition;
	color = vertexColor;
	uv = vec2(vertexUV.x, 1.0f -vertexUV.y); // negative v part to flip vertically 180 deg
}