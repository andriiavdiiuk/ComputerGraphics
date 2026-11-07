#include "Line3D.hpp"

#include <glm/geometric.hpp>

static void addLineToMesh(Mesh3D& mesh, const Line3D& line)
{
    const glm::vec3 direction = line.end - line.start;
    const float length = glm::length(direction);

    if (length <= 1e-6f)
        return;

    const glm::vec3 unitDirection = direction / length;
    const float halfWidth = line.width * 0.5f;

    const glm::vec3 reference =
        std::abs(unitDirection.y) < 0.9f
            ? glm::vec3(0, 1, 0)
            : glm::vec3(1, 0, 0);

    const glm::vec3 normal =
        glm::normalize(glm::cross(unitDirection, reference));

    const glm::vec3 binormal =
        glm::cross(unitDirection, normal);

    const glm::vec3 offset1 = normal * halfWidth;
    const glm::vec3 offset2 = binormal * halfWidth;

    const glm::vec3 vertices[] = {
        line.start - offset1 - offset2,
        line.start + offset1 - offset2,
        line.start + offset1 + offset2,
        line.start - offset1 + offset2,

        line.end - offset1 - offset2,
        line.end + offset1 - offset2,
        line.end + offset1 + offset2,
        line.end - offset1 + offset2
    };

    const unsigned base = static_cast<unsigned>(mesh.vertices.size());

    for (const auto& position : vertices)
    {
        mesh.vertices.push_back({
            .position = position,
            .color = line.color
        });
    }

    constexpr unsigned indices[] = {
        // Start cap
        0, 2, 1,
        0, 3, 2,

        // End cap
        4, 5, 6,
        4, 6, 7,

        // Side 1
        0, 1, 5,
        0, 5, 4,

        // Side 2
        1, 2, 6,
        1, 6, 5,

        // Side 3
        2, 3, 7,
        2, 7, 6,

        // Side 4
        3, 0, 4,
        3, 4, 7
    };

    for (const unsigned index : indices)
        mesh.indices.push_back(base + index);
}

Mesh3D generateMesh(const Line3D& line)
{
    Mesh3D mesh;
    addLineToMesh(mesh, line);
    return mesh;
}


Mesh3D generateMesh(const std::vector<Line3D>& lines)
{
    Mesh3D mesh;

    for (const Line3D& line : lines)
    {
        addLineToMesh(mesh, line);
    }

    return mesh;
}


