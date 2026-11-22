#include "IfsFractal.hpp"

#include <random>
#include <__msvc_ranges_to.hpp>
#include <glm/mat2x2.hpp>

#include "utils/GlmColors.hpp"

static glm::vec4 randomColor(const std::vector<glm::vec4>& colors)
{
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_real_distribution<float> colorDist(0.2f, 1.0f);
    return {
        colorDist(rng),
        colorDist(rng),
        colorDist(rng),
        1.0f
    };
}

Mesh2D generateMesh(const IfsFractalParams& params)
{
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    Mesh2D mesh;
    mesh.primitive = GL_POINTS;
    mesh.vertices.reserve(params.iterations);

    glm::vec2 position(0.0f);

    for (int iteration = 0; iteration < params.iterations; iteration++)
    {
        float r = dist(rng);
        float cumulative = 0.0f;

        for (size_t i = 0; i < params.entries.size(); i++)
        {
            const auto& entry = params.entries[i];

            cumulative += entry.p;

            if (r < cumulative || i == params.entries.size() - 1)
            {
                glm::mat2 mat{
                    entry.a, entry.b,
                    entry.c, entry.d
                };

                glm::vec2 translation{
                    entry.e, entry.f
                };

                position = mat * position + translation;

                Vertex<glm::vec2> vertex{};
                vertex.position = position;

                vertex.color = i < params.colors.size()? params.colors[i] : randomColor(params.colors);

                mesh.vertices.push_back(vertex);

                break;
            }
        }
    }

    return mesh;
}
