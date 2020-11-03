#version 460 core

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 texCoords;

uniform mat4 P;

void main()
{
    gl_Position.xy = (P * vec4(vertexPosition.xy, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
    texCoords = vec2(vertexUV.x, 1.0f -vertexUV.y); // negative v part to flip vertically 180 deg;
} 