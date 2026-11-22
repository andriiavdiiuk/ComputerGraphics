#include "graphics/meshes/Mesh.hpp"

#include <glm/geometric.hpp>

#include "transformations/Transformation.hpp"

template <typename Ver>
constexpr GLint getDimensions()
{
    using T = decltype(std::declval<Ver>().position);

    if constexpr (std::is_same_v<T, glm::vec2>)
        return 2;
    else if constexpr (std::is_same_v<T, glm::vec3>)
        return 3;
    else
        static_assert(sizeof(T) == 0, "Unsupported position type");

    return 0;
}

template <typename Ver>
void Mesh<Ver>::upload() const
{
    vao.bind();

    vbo.bind();
    VertexBufferObject::setBufferData(vertices.data(), vertices.size());

    ebo.bind();
    IndexBufferObject::setBufferData(indices.data(), indices.size());
    const auto positionComponents = getDimensions<Ver>();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        positionComponents,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Ver),
        reinterpret_cast<void*>(offsetof(Ver, position))
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Ver),
        reinterpret_cast<void*>(offsetof(Ver, color))
    );

    VertexBufferObject::unbind();
    IndexBufferObject::unbind();
    VertexArrayObject::unbind();
}

template <typename Ver>
void Mesh<Ver>::setupShader(sf::Shader& shader, const sf::Glsl::Mat4& projectionView, const sf::Glsl::Mat4& model) const
{
    sf::Shader::bind(&shader);
    shader.setUniform("projectionView", projectionView);
    shader.setUniform("model", model);
}

template <typename Ver>
void Mesh<Ver>::render() const
{
    vao.bind();
    if (!indices.empty()) {
        ebo.bind();

        glDrawElements(
            primitive,
            static_cast<GLsizei>(indices.size()),
            GL_UNSIGNED_INT,
            nullptr
        );

        IndexBufferObject::unbind();
    }
    else {
        glDrawArrays(
            primitive,
            0,
            static_cast<GLsizei>(vertices.size())
        );
    }

    VertexBufferObject::unbind();
    VertexArrayObject::unbind();
}


template <typename Ver>
void Mesh<Ver>::render(sf::Shader& shader, const sf::Glsl::Mat4& projectionView, const sf::Glsl::Mat4& model) const
{
    setupShader(shader, projectionView, model);
    render();
}

template struct Mesh<Vertex<glm::vec2>>;
template struct Mesh<Vertex<glm::vec3>>;