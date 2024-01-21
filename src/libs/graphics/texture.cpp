#include "texture.hpp"

namespace graphics
{

static uint32_t getPixelFormatGL(Texture::Format format)
{
    switch (format)
    {
    case Texture::Format::R8:
    case Texture::Format::R16F:
        return GL_RED;
    case Texture::Format::RG8:
    case Texture::Format::RG16F:
        return GL_RG;
    case Texture::Format::RGB8:
    case Texture::Format::RGB16F:
        return GL_RGB;
    case Texture::Format::RGBA8:
    case Texture::Format::RGBA16F:
    case Texture::Format::DXT1:
    case Texture::Format::DXT5:
        return GL_RGBA;
    case Texture::Format::BGR8:
        return GL_BGR;
    case Texture::Format::BGRA8:
        return GL_BGRA;
    case Texture::Format::Depth24:
    case Texture::Format::Depth32F:
        return GL_DEPTH_COMPONENT;
    case Texture::Format::Depth32FStencil8:
        return GL_DEPTH_STENCIL;
    default:
        throw std::invalid_argument("Invalid pixel format \"" +
                                    std::to_string(static_cast<int>(format)) + "\"");
    }
}

static uint32_t getPixelTypeGL(Texture::Format format)
{
    switch (format)
    {
    case Texture::Format::R8:
    case Texture::Format::RG8:
    case Texture::Format::RGB8:
    case Texture::Format::RGBA8:
    case Texture::Format::BGR8:
    case Texture::Format::BGRA8:
        return GL_UNSIGNED_BYTE;
    case Texture::Format::Depth24:
        return GL_UNSIGNED_INT;
    case Texture::Format::R16F:
    case Texture::Format::RG16F:
    case Texture::Format::RGB16F:
    case Texture::Format::RGBA16F:
    case Texture::Format::Depth32F:
        return GL_FLOAT;
    case Texture::Format::Depth32FStencil8:
        return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
    default:
        throw std::invalid_argument("Invalid pixel format \"" +
                                    std::to_string(static_cast<int>(format)) + "\"");
    }
}

static uint32_t getFilterGL(Texture::FilterMode filter)
{
    switch (filter)
    {
    case Texture::FilterMode::NEAREST:
        return GL_NEAREST;
    case Texture::FilterMode::NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
    case Texture::FilterMode::LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;
    case Texture::FilterMode::NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
    case Texture::FilterMode::LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;
    case Texture::FilterMode::LINEAR:
    default:
        return GL_LINEAR;
    }
}

Texture::Texture()
{
    glGenTextures(1, &_id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_id);
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::setFilterMode(FilterMode min, FilterMode mag)
{
    _minFilter = min;
    _magFilter = mag;
}

void Texture::setWrapMode(WrapMode s, WrapMode t)
{
    _wrapS = s;
    _wrapT = t;
}

void Texture::setPixels(int w, int h, Format format, std::vector<uint8_t> pixels, bool refresh)
{
    _width = w;
    _height = h;
    _format = format;

    _pixels = std::move(pixels);

    if (refresh)
        this->refresh();
}

void Texture::refresh()
{
    switch (_format)
    {
    case Format::DXT1:
    case Format::DXT5:
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, getPixelFormatGL(_format), _width, _height, 0,
                               _pixels.size(), _pixels.data());
        break;
    default:
        glTexImage2D(GL_TEXTURE_2D, 0, getPixelFormatGL(_format), _width, _height, 0,
                     getPixelFormatGL(_format), getPixelTypeGL(_format), _pixels.data());
        break;
    }
}

} // namespace graphics