#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform vec3 aColor;
uniform bool useTexture;

void main()
{
	if (useTexture)
	{
		FragColor = texture(texture0, TexCoord);
	}
	else
	{
		FragColor = vec4(aColor, 1.0);
	}
}