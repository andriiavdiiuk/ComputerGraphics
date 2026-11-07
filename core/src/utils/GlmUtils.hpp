#pragma once
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
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

inline sf::Glsl::Mat3 toSfmlMat(const glm::mat3& matrix)
{
    return sf::Glsl::Mat3(glm::value_ptr(matrix));
}

inline sf::Glsl::Mat4 toSfmlMat(const glm::mat4& matrix)
{
    return sf::Glsl::Mat4(glm::value_ptr(matrix));
}