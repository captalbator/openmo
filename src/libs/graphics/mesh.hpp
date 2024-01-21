#pragma once

namespace graphics
{

// TODO: add more types
enum class VertexType
{
    FLOAT,
};

class VertexLayout
{
public:
    struct Element
    {
        uint32_t offset;
        uint32_t location;
        GLenum type;
    };

    VertexLayout() {}
    ~VertexLayout() {}

    void appendElement(uint32_t size, VertexType type);

    int getStride() const { return _stride; }
    const std::vector<Element> &getElements() const { return _elements; }

private:
    uint32_t _stride{0};
    std::vector<Element> _elements;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void setVertices(std::vector<float> vertices);

    void setLayout(VertexLayout layout);

    void draw();
    void drawInstanced(int count);

private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;

    VertexLayout _layout;

    std::vector<float> _vertices;
};

} // namespace graphics