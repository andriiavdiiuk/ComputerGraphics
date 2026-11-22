#pragma once
#include <vector>

#include "graphics/meshes/Mesh.hpp"

struct IfsFractalEntry
{
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;

    float p;
};

struct IfsFractalParams
{
    std::vector<IfsFractalEntry> entries;
    int iterations;
    std::vector<glm::vec4> colors;
};

Mesh2D generateMesh(const IfsFractalParams& params);