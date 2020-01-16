#version 150

uniform mat4 camera;

in vec3 vert;

void main()
{
    gl_Position = camera * vec4(vert, 1);
}