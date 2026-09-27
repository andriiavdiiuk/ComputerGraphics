#include "Circle.hpp"

#include <numbers>
#include "graphics/meshes/Mesh.hpp"

Mesh generateMesh(const Circle& circle)
{
   Mesh mesh;

    const float half_width = circle.width / 2.f;
    const float outer_radius = circle.radius + half_width;
    const float inner_radius = circle.radius - half_width;

    mesh.vertices.reserve(circle.segments * 2);
    mesh.indices.reserve(circle.segments * 6);

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
        const unsigned int current = i * 2;
        const unsigned int next = ((i + 1) % circle.segments) * 2;

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

    return mesh;
}
