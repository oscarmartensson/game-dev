#version 130

// From vertex shader. Important to use same name
in vec2 position;
in vec4 color;
in vec2 uv;

// Output
out vec4 fragmentColor;

// Uniforms
uniform float time;
uniform sampler2D texSampler;

// ----------------------------------
// Main
void main()
{

	vec4 texColor = texture(texSampler, uv);

	fragmentColor = texColor * color;

	fragmentColor = vec4(color.r * (cos(position.x + time) + 1.0) * 0.5,
						color.g * (cos(position.y + time) + 1.0) * 0.5, 
						color.b * (cos(position.y + time) + 1.0) * 0.5, 
						1.0) * fragmentColor;
}