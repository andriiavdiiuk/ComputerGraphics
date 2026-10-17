#pragma once
#include <glm/vec2.hpp>

#include "Circle.hpp"
#include "Line.hpp"
#include "graphics/meshes/Mesh.hpp"

struct Circle;

struct ThirdDegreeCurveRenderData
{
    Mesh curveMesh;
    Mesh controlPointsMesh;
    Mesh controlPointLinesMesh;
};

struct ThirdDegreeCurve
{
    size_t point_a;
    size_t point_b;
    size_t point_c;
    size_t point_d;

    float weight_a;
    float weight_b;
    float weight_c;
    float weight_d;

    float step = 0.1;
    float width = 1;
    glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 controlPointLinesColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
};

struct ThirdDegreeCurvePath
{
    std::vector<Circle> points;
    std::vector<ThirdDegreeCurve> curves;
    bool enclosed;
};

struct ThirdDegreeCurves
{
    std::vector<ThirdDegreeCurvePath> paths;
};

ThirdDegreeCurveRenderData generateMesh(const ThirdDegreeCurvePath& curve);
ThirdDegreeCurveRenderData generateMesh(const ThirdDegreeCurves& curves);

void animateCurve(
    const ThirdDegreeCurves& original_curves,
    const ThirdDegreeCurves& target_curves,
    ThirdDegreeCurves& current_curves,
    float t
);
