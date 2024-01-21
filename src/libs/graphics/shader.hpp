#pragma once

namespace graphics
{

enum class ShaderType
{
    VERTEX,
    FRAGMENT,
};

class Shader
{
public:
    Shader(ShaderType shaderType);
    ~Shader();

    void setSource(std::filesystem::path srcPath);
    void setSource(const char **src);

    void compile();

    void destroy();

    GLuint getId() const { return _id; }

private:
    GLuint _id{0};
};

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void attach(Shader *shader);
    void link();

    void use();

    void destroy();

    // TODO: Add shader program management

private:
    GLuint _id{0};
};

} // namespace graphics