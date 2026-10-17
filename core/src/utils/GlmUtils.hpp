#pragma once
#include <glm/vec2.hpp>
#include <SFML/System/Vector2.hpp>

template <typename T>
inline glm::vec2 toGlmVec(sf::Vector2<T> v)
{
    return glm::vec2(v.x, v.y);
}

template <typename T>
inline glm::vec2 toGlmVec(sf::Vector3<T> v)
{
    return glm::vec2(v.x, v.y);
}
