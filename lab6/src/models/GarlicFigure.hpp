#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "primitives/Line3D.hpp"


struct GarlicFigure
{
    float r;
    float uStart;
    float uEnd;
    float uStep;
    float vStart;
    float vEnd;
    float vStep;
    float width;
    glm::vec4 color;

    glm::vec3 evaluatePoint(const float u, const float v) const;
    [[nodiscard]] std::vector<Line3D> toLines() const;
};
