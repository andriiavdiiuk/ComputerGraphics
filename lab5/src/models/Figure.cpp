#include "Figure.hpp"

#include <glm/gtc/constants.hpp>


std::vector<Line3D> Figure::toLines()
{
    std::vector<Line3D> lines;
    lines.reserve(30);

    constexpr int vertexCount = 10;

    constexpr float startAngle = -glm::half_pi<float>();
    constexpr float angleStep = glm::two_pi<float>() / vertexCount;

    glm::vec3 front[vertexCount];
    glm::vec3 back[vertexCount];

    for (int i = 0; i < vertexCount; ++i)
    {
        const float radius = (i % 2 == 0)
                                 ? outerRadius
                                 : innerRadius;

        const float angle = startAngle + i * angleStep;

        glm::vec3 offset(
            std::cos(angle) * radius,
            std::sin(angle) * radius,
            0.0f
        );

        front[i] = center + offset;
        back[i] = front[i] + glm::vec3(0.0f, 0.0f, height);
    }

    for (int i = 0; i < vertexCount; ++i)
    {
        lines.push_back({
            front[i],
            front[(i + 1) % vertexCount],
            width,
            color
        });
    }

    for (int i = 0; i < vertexCount; ++i)
    {
        lines.push_back({
            back[i],
            back[(i + 1) % vertexCount],
            width,
            color
        });
    }

    for (int i = 0; i < vertexCount; ++i)
    {
        lines.push_back({
            front[i],
            back[i],
            width,
            color
        });
    }

    return lines;
}
