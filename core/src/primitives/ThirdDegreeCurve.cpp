#include "ThirdDegreeCurve.hpp"

#include <glm/common.hpp>

glm::vec2 evaluate(const ThirdDegreeCurvePath& curvePath, const ThirdDegreeCurve& curve, float t)
{
    auto point_a = curvePath.points[curve.point_a].origin;
    auto point_b = curvePath.points[curve.point_b].origin;
    auto point_c = curvePath.points[curve.point_c].origin;
    auto point_d = curvePath.points[curve.point_d].origin;

    const float n = 1 - t;
    const float n_squared = n * n;
    const float n_cubed = n * n * n;
    const float a = curve.weight_a * n_cubed;
    const float b = 3 * curve.weight_b * t * n_squared;
    const float c = 3 * curve.weight_c * t * t * n;
    const float d = curve.weight_d * t * t * t;
    const glm::vec2 numerator = point_a * a + point_b * b + point_c * c + point_d * d;
    const float denominator = a + b + c + d;
    return numerator / denominator;
}

static void appendLines(const ThirdDegreeCurvePath& curvePath, std::vector<Line>& curveLines,
                        std::vector<Line>& controlPointLines,
                        std::vector<Circle>& controlPoints)
{
    for (auto curve : curvePath.curves)
    {
        glm::vec2 previous = evaluate(curvePath, curve, 0.0f);

        for (float t = curve.step; t < 1.0f; t += curve.step)
        {
            glm::vec2 current =  evaluate(curvePath, curve, t);

            curveLines.push_back({
                previous,
                current,
                curve.width,
                curve.color
            });

            previous = current;
        }

        curveLines.push_back({
            previous,
            evaluate(curvePath, curve, 1.0f),
            curve.width,
            curve.color
        });

        auto point_a = curvePath.points[curve.point_a];
        auto point_b = curvePath.points[curve.point_b];
        auto point_c = curvePath.points[curve.point_c];
        auto point_d = curvePath.points[curve.point_d];

        controlPointLines.push_back({
            point_a.origin,
            point_b.origin,
            curve.width,
            curve.controlPointLinesColor
        });

        controlPointLines.push_back({
            point_c.origin,
            point_d.origin,
            curve.width,
            curve.controlPointLinesColor
        });

        controlPoints.push_back(point_a);
        controlPoints.push_back(point_b);
        controlPoints.push_back(point_c);
        controlPoints.push_back(point_d);
    }
}

ThirdDegreeCurveRenderData generateMesh(const ThirdDegreeCurvePath& curve)
{
    std::vector<Line> curveLines;
    std::vector<Line> controlLines;
    std::vector<Circle> controlPoints;
    appendLines(curve, curveLines, controlLines, controlPoints);
    return {
        generateMesh(curveLines),
        generateMesh(controlLines),
        generateMesh(controlPoints),
    };
}

ThirdDegreeCurveRenderData generateMesh(const ThirdDegreeCurves& curves)
{
    std::vector<Line> curveLines;
    std::vector<Line> controlLines;
    std::vector<Circle> controlPoints;
    for (const auto& curvePath : curves.paths)
        appendLines(curvePath, curveLines, controlLines, controlPoints);

    return {
        generateMesh(curveLines),
        generateMesh(controlLines),
        generateMesh(controlPoints),
    };
}

void animateCurve(
    const ThirdDegreeCurves& original_curves,
    const ThirdDegreeCurves& target_curves,
    ThirdDegreeCurves& current_curves,
    float t)
{
    t = std::clamp(t, 0.0f, 1.0f);

    const size_t pathCount = std::min(
        original_curves.paths.size(),
        target_curves.paths.size()
    );

    for (size_t p = 0; p < pathCount; ++p)
    {
        const auto& originalPath = original_curves.paths[p];
        const auto& targetPath = target_curves.paths[p];

        const size_t pointCount = std::min(
            originalPath.points.size(),
            targetPath.points.size()
        );

        for (size_t i = 0; i < pointCount; ++i)
        {
            auto& currentPoint = current_curves.paths[p].points[i];

            const auto& originalPoint = originalPath.points[i];
            const auto& targetPoint = targetPath.points[i];

            currentPoint.origin = glm::mix(
                originalPoint.origin,
                targetPoint.origin,
                t
            );
        }
    }
}