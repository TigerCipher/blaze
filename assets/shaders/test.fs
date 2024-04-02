#version 330 core

uniform float red;
uniform float green;
uniform float blue;

out vec4 fragColor;

void main()
{
    fragColor = vec4(red, green, blue, 1.0);
}