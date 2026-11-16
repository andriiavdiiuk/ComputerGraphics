#include "GarlicFigure.hpp"

#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include "utils/GlmColors.hpp"

glm::vec3 GarlicFigure::evaluatePoint(const float u, const float v) const
{
    glm::vec3 point;

    constexpr auto pi = glm::pi<float>();

    const auto sin2u = std::abs(std::sin(2 * u));
    point.x = r * std::cos(u) * std::cos(v) * (1.f + 0.5f * sin2u);
    point.y = r * std::sin(u) * std::cos(v) * (1.f + 0.5f * sin2u);
    if (v > 0)
    {
        point.z = static_cast<float>(r * std::sin(v) + r * std::pow((v / (0.5 * pi)),5));
    }
    else
    {
        point.z = r * std::sin(v);
    }

    return point;
}
std::vector<Line3D> GarlicFigure::toLines() const
{
    std::vector<Line3D> lines;

    const int uSegments = static_cast<int>(std::round((uEnd - uStart) / uStep));
    const int vSegments = static_cast<int>(std::round((vEnd - vStart) / vStep));
    const float uSegmentsFloat = static_cast<float>(uSegments);
    const float vSegmentsFloat = static_cast<float>(vSegments);

    for (int i = 0; i <= uSegments; ++i)
    {
        const float iFloat = static_cast<float>(i);
        const float u = uStart + (uEnd - uStart) *  iFloat / uSegmentsFloat;

        for (int j = 0; j <= vSegments; ++j)
        {
            const float jFloat = static_cast<float>(j);
            const float v = vStart + (vEnd - vStart) * jFloat / vSegmentsFloat;

            glm::vec3 current = evaluatePoint(u, v);

            if (j < vSegments)
            {
                const float nextV = vStart + (vEnd - vStart) * (jFloat + 1) / vSegmentsFloat;

                lines.emplace_back(
                    current, evaluatePoint(u, nextV), width, color
                );
            }

            if (i < uSegments)
            {
                const float nextU = uStart + (uEnd - uStart) * (iFloat + 1) / uSegmentsFloat;

                lines.emplace_back(
                    current, evaluatePoint(nextU, v), width, color
                );
            }
            else
            {
                lines.emplace_back(
                    current, evaluatePoint(uStart, v), width, color
                );
            }
        }
    }

    return lines;
}