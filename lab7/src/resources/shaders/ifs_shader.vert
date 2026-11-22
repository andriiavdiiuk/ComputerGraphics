#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 vertex_color;

uniform mat4 projectionView;
uniform mat4 model;
uniform float pointSize;
uniform float zoomLevel;

void main()
{
    vec4 point = model * vec4(position, 0, 1);
    point.xyz /= point.w;
    gl_Position = projectionView * vec4(point.xyz, 1.0);
    gl_PointSize = pointSize * zoomLevel;
    vertex_color = color;
}
