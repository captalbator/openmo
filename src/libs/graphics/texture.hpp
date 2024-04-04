#pragma once

namespace graphics
{

class Texture
{
public:
    enum class FilterMode
    {
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST,
        LINEAR_MIPMAP_LINEAR,
    };

    enum class Format
    {
        R8,
        R16F,
        RG8,
        RG16F,
        RGB8,
        RGB16F,
        RGBA8,
        RGBA16F,
        DXT1,
        DXT5,
        BGR8,
        BGRA8,
        Depth24,
        Depth32F,
        Depth32FStencil8,
    };

    enum class WrapMode
    {
        CLAMP_TO_BORDER,
        CLAMP_TO_EDGE,
        REPEAT,
    };

    Texture();
    ~Texture();

    void bind();

    void setFilterMode(FilterMode min, FilterMode mag);
    void setWrapMode(WrapMode wrap);

    void setPixels(int w, int h, Format format, std::vector<uint8_t> pixels, bool refresh = false);

    void refresh();
    void configure();
    void generateMipmap();

    GLuint getId() const { return _id; }

private:
    GLuint _id;

    WrapMode _wrap{WrapMode::REPEAT};
    Format _format{Format::RGBA8};
    FilterMode _minFilter{FilterMode::LINEAR};
    FilterMode _magFilter{FilterMode::LINEAR};

    int _width;
    int _height;
    std::vector<uint8_t> _pixels;
};

} // namespace graphics