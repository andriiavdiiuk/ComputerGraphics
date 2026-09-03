#pragma once
#include <cstddef>
#include <SFML/OpenGL.hpp>

class VertexBufferObject
{
public:
    VertexBufferObject()
    {
        glGenBuffers(1, &id);
    }
    ~VertexBufferObject()
    {
        if (id != 0)
            glDeleteBuffers(1, &id);
    }

    VertexBufferObject(const VertexBufferObject&) = delete;
    VertexBufferObject& operator=(const VertexBufferObject&) = delete;

    VertexBufferObject(VertexBufferObject&& other) noexcept: id(other.id)
    {
        other.id = 0;
    }
    VertexBufferObject& operator=(VertexBufferObject&& other) noexcept
    {
        if (this != &other)
        {
            if (id != 0)
                glDeleteBuffers(1, &id);

            id = other.id;
            other.id = 0;
        }

        return *this;
    }

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }
    static void unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    template<typename T>
    static void setBufferData(const T* data, const std::size_t count)
    {
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), data, GL_STATIC_DRAW);
    }

private:
    GLuint id{};
};
