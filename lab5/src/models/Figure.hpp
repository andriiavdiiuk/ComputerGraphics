#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "primitives/Line3D.hpp"


struct Figure
{
    glm::vec3 center;
    float outerRadius;
    float innerRadius;
    float height;
    float width;
    glm::vec4 color;

    std::vector<Line3D> toLines();
};
