#version 330 core

out vec4 FragColor;

in vec4 customColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	FragColor = mix(texture(texture0, TexCoord), customColor, 0.2f);
}