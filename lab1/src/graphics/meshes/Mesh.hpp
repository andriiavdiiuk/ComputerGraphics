#pragma once
#include <vector>
#include <glad/glad.h>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/View.hpp>
#include "Vertex.hpp"
#include "graphics/buffers/IndexBufferObject.hpp"
#include "graphics/buffers/VertexArrayObject.hpp"
#include "graphics/buffers/VertexBufferObject.hpp"
#include "transformations/Transformation.hpp"

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    GLenum primitive = GL_TRIANGLES;

    VertexBufferObject vbo;
    IndexBufferObject ebo;
    VertexArrayObject vao;

    void upload() const;

    void render(sf::Shader& shader, const sf::View& view, const sf::Glsl::Mat3& transform) const;
};
