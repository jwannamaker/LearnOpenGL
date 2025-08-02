#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 customColor;

void main()
{
	FragColor = customColor;
}