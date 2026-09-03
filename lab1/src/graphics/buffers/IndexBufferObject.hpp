#pragma once
#include <cstddef>
#include <SFML/OpenGL.hpp>


class IndexBufferObject
{
public:

    IndexBufferObject()
    {
        glGenBuffers(1, &id);
    }
    ~IndexBufferObject()
    {
        if (id != 0)
            glDeleteBuffers(1, &id);
    }

    IndexBufferObject(const IndexBufferObject&) = delete;
    IndexBufferObject& operator=(const IndexBufferObject&) = delete;
    IndexBufferObject(IndexBufferObject&& other) noexcept: id(other.id)
    {
        other.id = 0;
    }
    IndexBufferObject& operator=(IndexBufferObject&& other) noexcept
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }
    static void unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    template<typename T>
    static void setBufferData(const T* indices, const std::size_t count)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(T), indices, GL_STATIC_DRAW);
    }

private:
    GLuint id{};
};
