#include "transformations/Transformation.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "primitives/Circle.hpp"
#include "graphics/meshes/Mesh.hpp"

AffineTransformation::AffineTransformation() :
    origin(0,0),
    unitX(1.0f, 0),
    unitY(0, 1.0f),
    scale(1.0f, 1.0f),
    symmetryPoint(500, 500),
    isApplySymmetry(false)
{}

EuclideanTransformation::EuclideanTransformation() :
    offset(0.0f, 0.0f),
    rotationPoint(0.0f, 0.0f),
    rotationDegrees(0)
{}

ProjectiveTransformation::ProjectiveTransformation() :
    origin(0.0f, 0.0f),
    xAxisEnd(600, 0.0f),
    yAxisEnd(0, 600.0f),
    weights(1,1, 600)
{}

glm::mat3 AffineTransformation::getMatrix() const
{
    const glm::mat3 affineMatrix = {
        unitX.x, unitY.x, 0,
        unitX.y, unitY.y, 0,
        origin.x, origin.y, 1
    };

    const glm::mat3 scaleMatrix = {
        scale.x, 0, 0,
        0, scale.y, 0,
        0, 0, 1,
    };

    const glm::mat3 symmetryMatrix = {
        -1, 0, 0,
        0, -1, 0,
        2 * symmetryPoint.x, 2 * symmetryPoint.y, 1
    };

    glm::mat3 resultMatrix = affineMatrix * scaleMatrix;
    if (isApplySymmetry)
    {
        resultMatrix *= symmetryMatrix;
    }

    return resultMatrix;
}


glm::mat3 EuclideanTransformation::getMatrix() const
{
    const glm::mat3 offsetMatrix = {
        1, 0, 0,
        0, 1, 0,
        offset.x, offset.y, 1
    };

    const float a = glm::radians(rotationDegrees);
    const float cosa = cos(a);
    const float sina = sin(a);
    const float rotationValue1 = -rotationPoint.x * (cosa - 1) + rotationPoint.y * sina;
    const float rotationValue2 = -rotationPoint.x * sina - rotationPoint.y * (cosa - 1);

    const glm::mat3 rotationMatrix = {
        cosa, sina, 0,
        -sina, cosa, 0,
        rotationValue1, rotationValue2, 1
    };

    return offsetMatrix * rotationMatrix;
}

glm::mat3 ProjectiveTransformation::getMatrix() const
{
    const glm::mat3 m = {
        xAxisEnd.x * weights.x, xAxisEnd.y * weights.x, weights.x,
        yAxisEnd.x * weights.y, yAxisEnd.y * weights.y, weights.y,
        origin.x * weights.z, origin.y * weights.z, weights.z,
    };

    return m;
}

Mesh EuclideanTransformation::getRotationPointMesh(float radius , float width) const
{
    const Circle c = {
        rotationPoint,
        radius,
        width,
        32,
        {0, 0, 0, 1}
    };
    return generateMesh(c);
}


sf::Glsl::Mat3 toSfmlMat3(const glm::mat3& matrix)
{
    return sf::Glsl::Mat3(glm::value_ptr(matrix));
}

sf::Transform toSfmlTransform(const glm::mat3& matrix)
{
    return {
        matrix[0][0], matrix[1][0], matrix[2][0],
        matrix[0][1], matrix[1][1], matrix[2][1],
        matrix[0][2], matrix[1][2], matrix[2][2]
    };
}


void resizeLinePoints(glm::vec2& pointA, glm::vec2& pointB, float newLength)
{
    newLength = std::max(newLength, 0.001f);

    const glm::vec2 midpoint = (pointA + pointB) / 2.0f;
    const glm::vec2 directionA = glm::normalize(pointA - midpoint);
    const glm::vec2 directionB = glm::normalize(pointB - midpoint);

    const float halfLength = newLength / 2.f;

    pointA = midpoint + directionA * halfLength;
    pointB = midpoint + directionB * halfLength;
}
