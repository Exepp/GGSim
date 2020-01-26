#version 330 core
in vec4 color;
out vec4 FragColor;

uniform bool forceBlack;

void main()
{
    if (forceBlack)
        FragColor = vec4(0, 0, 0, 1);
    else
        FragColor = color;
}