#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D koiTexture;
uniform bool darken;

void main()
{
	FragColor = texture(koiTexture, TexCoord);
	if (darken && FragColor.r > 0.975)
	{
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}