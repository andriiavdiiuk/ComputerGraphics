#include "Circle.hpp"

#include <numbers>
#include <glm/geometric.hpp>

#include "graphics/meshes/Mesh.hpp"

static void appendFilledCircle(const Circle& circle, Mesh& mesh)
{
    const auto baseIndex = static_cast<unsigned int>(mesh.vertices.size());

    mesh.vertices.push_back({
        circle.origin,
        circle.color
    });

    for (unsigned int i = 0; i < circle.segments; ++i)
    {
        const float angle =
            2.f * std::numbers::pi_v<float> *
            static_cast<float>(i) /
            static_cast<float>(circle.segments);

        mesh.vertices.push_back({
            {
                circle.origin.x + std::cos(angle) * circle.radius,
                circle.origin.y + std::sin(angle) * circle.radius
            },
            circle.color
        });
    }

    for (unsigned int i = 0; i < circle.segments; ++i)
    {
        mesh.indices.push_back(baseIndex);
        mesh.indices.push_back(baseIndex + 1 + i);
        mesh.indices.push_back(baseIndex + 1 + (i + 1) % circle.segments);
    }
}

static void appendCircle(const Circle& circle, Mesh& mesh)
{
    const float half_width = circle.width / 2.f;
    const float outer_radius = circle.radius + half_width;
    const float inner_radius = circle.radius - half_width;


    const auto baseIndex = static_cast<unsigned int>(mesh.vertices.size());

    for (unsigned int i = 0; i < circle.segments; ++i)
    {
        const float angle =
            2.f * std::numbers::pi_v<float> *
            static_cast<float>(i) /
            static_cast<float>(circle.segments);

        const float cos = std::cos(angle);
        const float sin = std::sin(angle);

        mesh.vertices.push_back({
            {
                circle.origin.x + cos * outer_radius,
                circle.origin.y + sin * outer_radius
            },
            circle.color
        });

        mesh.vertices.push_back({
            {
                circle.origin.x + cos * inner_radius,
                circle.origin.y + sin * inner_radius
            },
            circle.color
        });
    }

    for (unsigned int i = 0; i < circle.segments; ++i)
    {
        const unsigned int current = baseIndex + i * 2;
        const unsigned int next = baseIndex + ((i + 1) % circle.segments) * 2;

        const unsigned int outer_current = current;
        const unsigned int inner_current = current + 1;

        const unsigned int outer_next = next;
        const unsigned int inner_next = next + 1;

        // Triangle 1
        mesh.indices.push_back(outer_current);
        mesh.indices.push_back(outer_next);
        mesh.indices.push_back(inner_next);

        // Triangle 2
        mesh.indices.push_back(outer_current);
        mesh.indices.push_back(inner_next);
        mesh.indices.push_back(inner_current);
    }
}

Mesh generateMesh(const Circle& circle)
{
    Mesh mesh;

    if (circle.filled)
        appendFilledCircle(circle, mesh);
    else
        appendCircle(circle, mesh);
    return mesh;
}

Mesh generateMesh(const std::vector<Circle>& circles)
{
    Mesh mesh;

    for (const auto& circle : circles)
    {
        if (circle.filled)
            appendFilledCircle(circle, mesh);
        else
            appendCircle(circle, mesh);
    }
    return mesh;
}

bool Circle::containsPoint(const glm::vec2& point) const
{
    const glm::vec2 delta = point - origin;
    const float distanceSquared = glm::dot(delta, delta);

    if (filled)
        return distanceSquared <= radius * radius;

    const float halfWidth = width / 2.0f;
    const float innerRadius = std::max(0.0f, radius - halfWidth);
    const float outerRadius = radius + halfWidth;

    return distanceSquared >= innerRadius * innerRadius && distanceSquared <= outerRadius * outerRadius;
}
