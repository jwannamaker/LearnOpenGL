#version 330 core

out vec4 FragColor;

in vec3 customColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixAmount;

void main()
{
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), mixAmount);
}