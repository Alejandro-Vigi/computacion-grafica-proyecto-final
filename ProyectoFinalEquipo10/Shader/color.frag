#version 330 core

out vec4 color;

uniform vec3 objectColor;
uniform float alpha;

void main()
{
    color = vec4(objectColor, alpha);
}