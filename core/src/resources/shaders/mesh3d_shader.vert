#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 vertex_color;

uniform mat4 projectionView;
uniform mat4 model;

void main()
{
    vec4 point = model * vec4(position, 1);
    point.xyz /= point.w;
    gl_Position = projectionView * vec4(point.xyz,1);
    vertex_color = color;
}
