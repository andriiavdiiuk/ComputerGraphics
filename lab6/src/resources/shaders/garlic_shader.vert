#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 vertex_color;

uniform float scale;
uniform float r;
uniform mat3 localModel;
uniform mat4 projectionView;
uniform mat4 model;

const float PI = 3.14159265358979323846;
vec3 evaluatePoint(float u, float v)
{
    float sin2u = abs(sin(2.0 * u));

    vec3 point;

    point.x = r * cos(u) * cos(v) * (1.0 + 0.5 * sin2u);
    point.y = r * sin(u) * cos(v) * (1.0 + 0.5 * sin2u);

    if (v > 0.0)
    {
        point.z = r * sin(v)
        + r * pow(v / (0.5 * PI), 5.0);
    }
    else
    {
        point.z = r * sin(v);
    }

    return point;
}

void main()
{
    vec3 localPoint = localModel * vec3(position,1.0);
    localPoint.xy /= localPoint.z;
    vec2 parametricPosition = localPoint.xy * scale;
    vec3 projectedPoint = evaluatePoint(parametricPosition.x, parametricPosition.y);
    vec4 point = model * vec4(projectedPoint, 1);
    point.xyz /= point.w;
    gl_Position = projectionView * vec4(point.xyz, 1.0);
    vertex_color = color;
}
