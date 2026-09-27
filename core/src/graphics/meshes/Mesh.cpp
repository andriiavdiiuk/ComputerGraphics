#include "graphics/meshes/Mesh.hpp"
#include "transformations/Transformation.hpp"

void Mesh::upload() const
{
    vao.bind();

    vbo.bind();
    VertexBufferObject::setBufferData(vertices.data(), vertices.size());

    ebo.bind();
    IndexBufferObject::setBufferData(indices.data(), indices.size());


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position))
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, color))
    );

    VertexBufferObject::unbind();
    IndexBufferObject::unbind();
    VertexArrayObject::unbind();
}

void Mesh::render(sf::Shader& shader, const sf::View& view, const sf::Glsl::Mat3& transform) const
{
    sf::Shader::bind(&shader);
    const sf::Glsl::Mat4 viewMatrix(view.getTransform());
    shader.setUniform("view", viewMatrix);
    shader.setUniform("transformation", transform);
    vao.bind();
    ebo.bind();

    glDrawElements(
        primitive,
        static_cast<GLsizei>(indices.size()),
        GL_UNSIGNED_INT,
        nullptr
    );
    VertexBufferObject::unbind();
    IndexBufferObject::unbind();
    VertexArrayObject::unbind();
}