#include "mesh.hpp"
#include "libs/common/log.hpp"

namespace graphics
{

void VertexLayout::appendElement(uint32_t size, VertexType type)
{
    GLenum vertexType;
    switch (type)
    {
    case VertexType::FLOAT:
    default:
        vertexType = GL_FLOAT;
        break;
    }

    Element element;
    element.location = _elements.size();
    element.type = vertexType;
    element.offset = size;

    _elements.push_back(element);

    _stride += size;
}

Mesh::Mesh()
{
    glGenBuffers(1, &_vbo);
    glGenVertexArrays(1, &_vao);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void Mesh::setVertices(std::vector<float> vertices)
{
    _vertices = std::move(vertices);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);
}

void Mesh::setLayout(VertexLayout layout)
{
    _layout = std::move(layout);

    glBindVertexArray(_vao);

    for (auto &element : _layout.getElements())
    {
        glEnableVertexAttribArray(element.location);
        glVertexAttribPointer(element.location, element.offset, element.type, GL_FALSE,
                              _layout.getStride() * sizeof(element.type),
                              (void *)(element.location * element.offset * sizeof(element.type)));
    }

    glBindVertexArray(0);
}

void Mesh::draw()
{
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}

void Mesh::drawInstanced(int count)
{
}

} // namespace graphics