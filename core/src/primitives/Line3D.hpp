#pragma once
#include <glm/vec3.hpp>
#include "graphics/meshes/Mesh.hpp"

struct Line3D
{
    glm::vec3 start;
    glm::vec3  end;
    float width{};
    glm::vec4 color{};
};


Mesh3D generateMesh(const Line3D& line);
Mesh3D generateMesh(const std::vector<Line3D> &lines);


