#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 vertex_color;

uniform mat4 view;
uniform mat3 transformation;

void main()
{
    vec3 point = transformation * vec3(position, 1);
    point.xy /= point.z;
    gl_Position = view * vec4(point.xy, 0.0, 1.0);
    vertex_color = color;
}
