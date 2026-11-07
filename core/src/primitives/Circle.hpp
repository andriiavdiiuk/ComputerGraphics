#pragma once

#include "graphics/meshes/Mesh.hpp"

struct Circle
{
    glm::vec2 origin;
    float radius;
    float width;
    int segments;
    glm::vec4 color;
    bool filled;

    [[nodiscard]] bool containsPoint(const glm::vec2& point) const;
};

Mesh2D generateMesh(const Circle& circle);
Mesh2D generateMesh(const std::vector<Circle>& circles);
