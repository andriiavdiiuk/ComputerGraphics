#include "FoliumOfDescartes.hpp"

#include <glm/geometric.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "math/Derivative.hpp"
#include "primitives/Circle.hpp"
#include "primitives/Line.hpp"

FoliumOfDescartes FoliumOfDescartes::Default = {
    .a = 1,
    .angle_start = 0,
    .angle_end = glm::pi<float>(),
    .angle_step = 0.05,
    .line_width = 0.1,
    .color = {0.f, 0.f, 0.f, 1.f},
    .a_start = 0,
    .a_end = 10,
    .a_step = 0.01,
    .a_direction = 1,
    .include_asymptote = false,
    .asymptote_length = 100,
};

static double x_t(double a, double t, double eps)
{
    double denominator = 1.0 + t * t * t;

    if (std::abs(denominator) < eps)
        return std::numeric_limits<double>::quiet_NaN();

    return 3.0 * a * t / denominator;
}

static double y_t(double a, double t, double eps)
{
    double denominator = 1.0 + t * t * t;

    if (std::abs(denominator) < eps)
        return std::numeric_limits<double>::quiet_NaN();

    return 3.f * a * t * t / denominator;
}

static double curvature(double a, double t, double eps)
{
    auto x = [a, eps](double t)
    {
        return x_t(a, t, eps);
    };

    auto y = [a, eps](double t)
    {
        return y_t(a, t, eps);
    };

    const double xp = derivative(x, t, eps);
    const double yp = derivative(y, t, eps);

    const double xpp = second_derivative(x, t, eps);
    const double ypp = second_derivative(y, t, eps);

    const double numerator = xp * ypp - yp * xpp;

    const double denominator = std::pow(xp * xp + yp * yp, 1.5f);

    if (denominator < eps)
        return std::numeric_limits<double>::quiet_NaN();

    return numerator / denominator;
}


static double curvatureRadius(double a, double t, double eps)
{
    double curvature_val = curvature(a, t, eps);

    if (std::abs(curvature_val) < eps || std::isnan(curvature_val))
        return std::numeric_limits<double>::infinity();

    return std::abs(1 / curvature_val);
}

glm::vec2 FoliumOfDescartes::evaluate(const float angle) const
{
    const float tan_t = std::tan(angle);
    const float tan_squared = tan_t * tan_t;
    const float tan_cubed = tan_squared * tan_t;

    const float denominator = 1.f + tan_cubed;

    const float x = 3.f * a * tan_t / denominator;
    const float y = 3.f * a * tan_squared / denominator;

    return glm::vec2{x, y};
}

FoliumOfDescartesDrawData FoliumOfDescartes::toMesh() const
{
    std::vector<Line> lines;
    bool has_previous = false;
    glm::vec2 previous{};

    const float max_segment_length = 10.f * std::abs(a);

    float length = 0.f;
    float area = 3 * a * a / 2;

    for (float angle = angle_start; angle <= angle_end; angle += angle_step)
    {
        const glm::vec2 current = evaluate(angle);
        if (has_previous)
        {
            const float distance = glm::distance(previous, current);

            length += distance;
            if (distance <= max_segment_length)
            {
                lines.push_back({
                    .start = previous,
                    .end = current,
                    .width = line_width,
                    .color = color
                });
            }
        }

        previous = current;
        has_previous = true;
    }

    if (include_asymptote)
    {
        const glm::vec2 direction = glm::normalize(glm::vec2{1.f, -1.f});

        const glm::vec2 center{
            -a / 2.f,
            -a / 2.f
        };

        lines.push_back({
            .start = center - direction * asymptote_length,
            .end = center + direction * asymptote_length,
            .width = line_width,
            .color = {1.f, 0.5f, 0.f, 1.f}
        });
    }


    return {
        length,
        area,
        generateMesh(lines)
    };
}


CurvePoint FoliumOfDescartes::getCurvePointMeshes(CurvePointParams params) const
{
    const float t = std::tan(params.angle);
    const float t_squared = t * t;
    const float t_cubed = t_squared * t;
    const float denominator = (t_cubed + 1) * (t_cubed + 1);
    const float x_derivative = a * (3 - 6 * t_cubed) / denominator;
    const float y_derivative = -3 * a * t * (t_cubed - 2) / denominator;
    float derivative = y_derivative / x_derivative;

    glm::vec2 tangent{
        1.f,
        derivative
    };

    tangent = glm::normalize(tangent);

    const glm::vec2 normal{
        -tangent.y,
        tangent.x
    };

    const glm::vec2 point{
        3.f * a * t / (1.f + t_cubed),
        3.f * a * t_squared / (1.f + t_cubed)
    };

    CurvePoint result{
        .point = point,
        .curvatureRadius = curvatureRadius(a, t, 1e-8),
        .pointMesh = {},
        .lineMeshes = {},
    };

    std::vector<Line> lines;

    // Point
    result.pointMesh = generateMesh(Circle{
        .origin = point,
        .radius=line_width,
        .width = line_width,
        .segments = 32,
        .color = {1.f, 0.f, 1.f, 1.f}
    });

    if (params.includeTangent)
    {
        lines.push_back({
            .start = point - tangent * params.length,
            .end = point + tangent * params.length,
            .width = line_width,
            .color = {0.f, 0.f, 1.f, 1.f}
        });
    }

    if (params.includeNormal)
    {
        lines.push_back(Line{
            .start = point - normal * params.length,
            .end = point + normal * params.length,
            .width = line_width,
            .color = {0.f, 1.f, 0.f, 1.f}
        });
    }

    result.lineMeshes = generateMesh(lines);
    return result;
}

void FoliumOfDescartes::animateNext()
{
    a += a_direction * a_step;

    a = std::clamp(a, a_start, a_end);

    if (a == a_end)
        a_direction = -1;
    else if (a == a_start)
        a_direction = 1;
}
