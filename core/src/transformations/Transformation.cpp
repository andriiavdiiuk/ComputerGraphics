#include "transformations/Transformation.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "primitives/Circle.hpp"
#include "graphics/meshes/Mesh.hpp"

AffineTransformation::AffineTransformation() :
    origin(0, 0),
    unitX(1.0f, 0),
    unitY(0, 1.0f),
    scale(1.0f, 1.0f),
    symmetryPoint(500, 500),
    isApplySymmetry(false)
{
}

EuclideanTransformation::EuclideanTransformation() :
    offset(0.0f, 0.0f),
    rotationPoint(0.0f, 0.0f),
    rotationDegrees(0)
{
}

ProjectiveTransformation::ProjectiveTransformation() :
    origin(0.0f, 0.0f),
    xAxisEnd(600, 0.0f),
    yAxisEnd(0, 600.0f),
    weights(1, 1, 600)
{
}

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

Mesh2D EuclideanTransformation::getRotationPointMesh(float radius, float width) const
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

glm::mat4 EuclideanTransformation3D::getMatrix() const
{
    const glm::mat4 offsetMatrix = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        offset.x, offset.y, offset.z, 1
    };

    const float x = glm::radians(rotationDegrees.x);
    const float y = glm::radians(rotationDegrees.y);
    const float z = glm::radians(rotationDegrees.z);
    const float cosx = cos(x);
    const float sinx = sin(x);
    const float cosy = cos(y);
    const float siny = sin(y);
    const float cosz = cos(z);
    const float sinz = sin(z);

    const glm::mat4 translateToPivot = glm::translate(glm::mat4(1.0f), rotationPoint);

    const glm::mat4 translateFromPivot = glm::translate(glm::mat4(1.0f), -rotationPoint);

    const glm::mat4 rotationMatrixX = {
        1, 0, 0, 0,
        0, cosx, -sinx, 0,
        0, sinx, cosx, 0,
        0, 0, 0, 1,
    };

    const glm::mat4 rotationMatrixY = {
        cosy, 0, siny, 0,
        0, 1, 0, 0,
        -siny, 0, cosy, 0,
        0, 0, 0, 1
    };

    const glm::mat4 rotationMatrixZ = {
        cosz, -sinz, 0, 0,
        sinz, cosz, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };

    const glm::mat4 rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
    return offsetMatrix * translateToPivot * rotationMatrix * translateFromPivot;
}

glm::mat4 DimetricTransformation3D::getMatrix() const
{
    const float a = glm::radians(rotationAngleDegrees);
    const float sina = sin(a);
    const float sinaSquared = sina * sina;
    const float cosa = cos(a);
    const float factor = std::sqrt(1 + sinaSquared);

    const glm::mat4 rotationMatrix = {
        1 / factor, 0, 0, 0,
        cosa * sina / factor, 0, sina, 0,
        -sinaSquared / factor, 0, cosa, 0,
        0, 0, 0, 1
    };

    return rotationMatrix;
}
