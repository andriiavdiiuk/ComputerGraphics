#pragma once
#include <SFML/OpenGL.hpp>

class VertexArrayObject
{
public:
    VertexArrayObject()
    {
        glGenVertexArrays(1, &id);
    }
    ~VertexArrayObject()
    {
        if (id != 0)
            glDeleteVertexArrays(1, &id);
    }

    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject& operator=(const VertexArrayObject&) = delete;
    VertexArrayObject(VertexArrayObject&& other) noexcept: id(other.id)
    {
        other.id = 0;
    }
    VertexArrayObject& operator=(VertexArrayObject&& other) noexcept
    {
        if (this != &other)
        {
            if (id != 0) glDeleteVertexArrays(1, &id);

            id = other.id;
            other.id = 0;
        }

        return *this;
    }

    void bind() const
    {
        glBindVertexArray(id);
    }
    static void unbind()
    {
        glBindVertexArray(0);
    }

private:
    GLuint id{};
};
