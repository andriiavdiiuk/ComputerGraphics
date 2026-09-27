#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "graphics/meshes/Mesh.hpp"

struct Line
{
    glm::vec2 start;
    glm::vec2 end;
    float width{};
    glm::vec4 color;
};


Mesh generateMesh(const Line& line);
Mesh generateMesh(const std::vector<Line> &lines);