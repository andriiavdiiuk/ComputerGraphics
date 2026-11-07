#include "Line.hpp"

#include <vector>
#include <glm/geometric.hpp>


static glm::vec3 getNormal(const glm::vec3& direction, const glm::vec3& reference = glm::vec3(0, 1, 0))
{
    glm::vec3 normal = glm::cross(direction, reference);

    if (glm::dot(normal, normal) < 1e-6f)
        normal = glm::cross(direction, glm::vec3(1, 0, 0));

    return glm::normalize(normal);
}

static void addLineToMesh(Mesh2D& mesh, const Line2D& line)
{
    const glm::vec2 direction = line.end - line.start;
    const float length = glm::length(direction);

    if (length == 0.0f)
        return;

    const glm::vec2 unitDirection = direction / length;
    const glm::vec2 normal =     {
        -direction.y,
         direction.x
    };

    const float halfWidth = line.width * 0.5f;

    const glm::vec2 extension = unitDirection * halfWidth;
    const glm::vec2 offset = normal * halfWidth;

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


Mesh2D generateMesh(const Line2D& line)
{
    Mesh2D mesh;
    addLineToMesh(mesh, line);
    return mesh;
}


Mesh2D generateMesh(const std::vector<Line2D>& lines)
{
    Mesh2D mesh;

    for (const Line2D& line : lines)
    {
        addLineToMesh(mesh, line);
    }

    return mesh;
}


bool Line2D::containsPoint(const glm::vec2& point) const
{
    const glm::vec2 direction = end - start;
    const float lengthSquared = glm::dot(direction, direction);

    if (lengthSquared == 0.0f)
    {
        const glm::vec2 delta = point - start;
        return glm::dot(delta, delta) <= (width * width) / 4.0f;
    }

    const float t = glm::clamp(
        glm::dot(point - start, direction) / lengthSquared,
        0.0f,
        1.0f
    );

    const glm::vec2 closest = start + t * direction;
    const glm::vec2 delta = point - closest;

    return glm::dot(delta, delta) <= (width * width) / 4.0f;
}