#pragma once
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/Transform.hpp>
#include "graphics/meshes/Mesh.hpp"

struct Mesh;

struct AffineTransformation
{
    glm::vec2 origin;
    glm::vec2 unitX;
    glm::vec2 unitY;
    glm::vec2 scale;
    glm::vec2 symmetryPoint;
    bool isApplySymmetry;

    AffineTransformation();
    [[nodiscard]] glm::mat3 getMatrix() const;
};


struct EuclideanTransformation
{
    glm::vec2 offset;
    glm::vec2 rotationPoint;
    float rotationDegrees;
    EuclideanTransformation();

    [[nodiscard]] glm::mat3 getMatrix() const;
    [[nodiscard]] Mesh getRotationPointMesh(float radius = 3, float width = 5) const;
};

struct ProjectiveTransformation
{
    glm::vec2 origin;
    glm::vec2 xAxisEnd;
    glm::vec2 yAxisEnd;
    glm::vec3 weights;

    ProjectiveTransformation();
    [[nodiscard]] glm::mat3 getMatrix() const;
};


sf::Glsl::Mat3 toSfmlMat3(const glm::mat3& matrix);
sf::Transform toSfmlTransform(const glm::mat3& matrix);
void resizeLinePoints(glm::vec2& pointA, glm::vec2& pointB, float newLength);
