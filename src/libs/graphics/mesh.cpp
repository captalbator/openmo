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
    glGenBuffers(1, &_ibo);
    glGenVertexArrays(1, &_vao);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ibo);
    glDeleteVertexArrays(1, &_vao);
}

void Mesh::setVertices(std::vector<float> vertices)
{
    _vertices = std::move(vertices);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);
}

void Mesh::setIndices(std::vector<uint16_t> indices)
{
    _indices = std::move(indices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint16_t), &_indices[0],
                 GL_STATIC_DRAW);
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

void Mesh::setTexture(std::unique_ptr<Texture> &texture)
{
    _texture.reset(texture.get());
}

void Mesh::draw()
{
    if (_texture.get())
        _texture->bind();

    glBindVertexArray(_vao);
    if (_indices.size() > 0)
    {
        glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_SHORT, nullptr);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, _vertices.size() / (_layout.getStride() / sizeof(float)));
    }
}

void Mesh::drawInstanced(int count)
{
    if (_texture.get())
        _texture->bind();

    glBindVertexArray(_vao);
    if (_indices.size() > 0)
    {
        glDrawElementsInstanced(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_SHORT, nullptr, count);
    }
    else
    {
        glDrawArraysInstanced(GL_TRIANGLES, 0,
                              _vertices.size() / (_layout.getStride() / sizeof(float)), count);
    }
}

} // namespace graphics