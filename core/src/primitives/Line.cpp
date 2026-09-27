
#include "Line.hpp"

#include <vector>

static void addLineToMesh(Mesh& mesh, const Line& line)
{
    const glm::vec2 direction = line.end - line.start;

    // length of direction
    const float length = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    if (length == 0.f)
        return;

    // Normalize direction to get unit vector
    const glm::vec2 unit_direction = direction / length;

    // Perpendicular unit vector used to offset the line
    // on both sides and give it thickness.
    const glm::vec2 normal{
        -unit_direction.y,
         unit_direction.x
    };

    const float half_width = line.width * 0.5f;

    // Extend the line along its direction to avoid gaps.
    const glm::vec2 extension = unit_direction * half_width;
    const glm::vec2 offset = normal * half_width;

    const glm::vec2 start = line.start - extension;
    const glm::vec2 end = line.end + extension;

    const auto base = static_cast<unsigned>(mesh.vertices.size());

    const glm::vec2 vertices[] = {
        start + offset,
        start - offset,
        end   - offset,
        end   + offset
    };

    for (const auto& position : vertices)
    {
        mesh.vertices.push_back({
            .position = position,
            .color = line.color
        });
    }

    mesh.indices.insert(
        mesh.indices.end(),
        {
            base + 0,
            base + 1,
            base + 3,
            base + 1,
            base + 2,
            base + 3
        }
    );
}


Mesh generateMesh(const Line& line)
{
    Mesh mesh;
    addLineToMesh(mesh, line);
    return mesh;
}
Mesh generateMesh(const std::vector<Line>& lines)
{
    Mesh mesh;

    for (const Line& line : lines)
    {
        addLineToMesh(mesh, line);
    }

    return mesh;
}
