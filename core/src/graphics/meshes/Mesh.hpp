#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/View.hpp>
#include "Vertex.hpp"
#include "graphics/buffers/IndexBufferObject.hpp"
#include "graphics/buffers/VertexArrayObject.hpp"
#include "graphics/buffers/VertexBufferObject.hpp"

template <typename Ver>
struct Mesh
{
    std::vector<Ver> vertices;
    std::vector<uint32_t> indices;
    GLenum primitive = GL_TRIANGLES;

    VertexBufferObject vbo;
    IndexBufferObject ebo;
    VertexArrayObject vao;

    void upload() const;

    void render(sf::Shader& shader, const sf::Glsl::Mat4& projectionView, const sf::Glsl::Mat4& model) const;
};

extern template struct Mesh<Vertex<glm::vec2>>;
extern template struct Mesh<Vertex<glm::vec3>>;

using Mesh2D = Mesh<Vertex<glm::vec2>>;
using Mesh3D = Mesh<Vertex<glm::vec3>>;