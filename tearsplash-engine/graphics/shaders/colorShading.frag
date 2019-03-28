#version 130

// From vertex shader. Important to use same name
in vec2 position;
in vec4 color;
in vec2 uv;

// Output
out vec4 fragmentColor;

// Uniforms
uniform sampler2D texSampler;

// ----------------------------------
// Main
void main()
{

	vec4 texColor = texture(texSampler, uv);

	fragmentColor = texColor * color;
}