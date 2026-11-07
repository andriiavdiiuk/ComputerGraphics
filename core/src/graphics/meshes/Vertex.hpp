#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

template <typename Vec>
struct Vertex
{
    Vec position;
    glm::vec4 color;
};

template struct Vertex<glm::vec2>;
template struct Vertex<glm::vec3>;