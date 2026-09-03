#pragma once

#include "graphics/meshes/Mesh.hpp"

struct Circle
{
    glm::vec2 origin;
    float radius;
    float width;
    int segments;
    glm::vec4 color;
};

Mesh generateMesh(const Circle& circle);