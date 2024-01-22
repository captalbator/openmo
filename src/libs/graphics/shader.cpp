#include "shader.hpp"
#include "libs/common/log.hpp"

namespace graphics
{

std::string readEntireFile(std::filesystem::path filepath)
{
    std::string content;
    std::ifstream file_stream(filepath, std::ios::in);

    if (!file_stream.is_open())
    {
        LOG_ERROR("Could not read file {} : file does not exist", filepath.string().c_str());
        return std::string();
    }

    std::string line = "";
    while (!file_stream.eof())
    {
        std::getline(file_stream, line);
        content.append(line + "\n");
    }

    file_stream.close();
    return content;
}

Shader::Shader(ShaderType shaderType)
{
    GLenum glShaderType;
    switch (shaderType)
    {
    case ShaderType::VERTEX:
        glShaderType = GL_VERTEX_SHADER;
        break;
    case ShaderType::FRAGMENT:
        glShaderType = GL_FRAGMENT_SHADER;
        break;
    default:
        break;
    }
    _id = glCreateShader(glShaderType);
}

Shader::~Shader()
{
    destroy();
}

void Shader::setSource(const char **src)
{
    glShaderSource(_id, 1, src, NULL);
}

void Shader::setSource(std::filesystem::path srcPath)
{
    std::string fileSrc = readEntireFile(srcPath);
    if (fileSrc.empty())
    {
        LOG_ERROR("Failed to read shader file");
        return;
    }

    const char *shaderSrc = fileSrc.c_str();

    glShaderSource(_id, 1, &shaderSrc, NULL);
}

void Shader::compile()
{
    glCompileShader(_id);

    int result = 0;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        int logLen;
        glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &logLen);

        char *log = new char[logLen];
        glGetShaderInfoLog(_id, logLen, NULL, log);

        LOG_ERROR("Failed to compile shader: {}", log);

        delete[] log;
    }
}

void Shader::destroy()
{
    glDeleteShader(_id);
}

ShaderProgram::ShaderProgram()
{
    _id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    destroy();
}

void ShaderProgram::attach(Shader *shader)
{
    glAttachShader(_id, shader->getId());
}

void ShaderProgram::link()
{
    glLinkProgram(_id);

    int result = 0;
    glGetProgramiv(_id, GL_LINK_STATUS, &result);

    if (!result)
    {
        int logLen;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logLen);

        char *log = new char[logLen];
        glGetProgramInfoLog(_id, logLen, NULL, log);

        LOG_ERROR("Failed to link program: {}", log);

        delete[] log;
    }
}

void ShaderProgram::use()
{
    glUseProgram(_id);
}

void ShaderProgram::destroy()
{
    glDeleteProgram(_id);
}

void ShaderProgram::setUniform(const std::string &name, int value)
{
    this->setUniform(name, [this, &value](int location) { glUniform1i(location, value); });
}

void ShaderProgram::setUniform(const std::string &name, bool value)
{
    this->setUniform(name, [this, &value](int location) { glUniform1i(location, (int)value); });
}

void ShaderProgram::setUniform(const std::string &name, float value)
{
    this->setUniform(name, [this, &value](int location) { glUniform1f(location, value); });
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec2 &value)
{
    this->setUniform(name,
                     [this, &value](int location) { glUniform2f(location, value.x, value.y); });
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec3 &value)
{
    this->setUniform(name, [this, &value](int location)
                     { glUniform3f(location, value.x, value.y, value.z); });
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat4 &value)
{
    this->setUniform(name, [this, &value](int location)
                     { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)); });
}

void ShaderProgram::setUniform(const std::string &name, const std::vector<glm::mat4> &arr)
{
    setUniform(name,
               [this, &arr](int location)
               {
                   glUniformMatrix4fv(location, static_cast<GLsizei>(arr.size()), GL_FALSE,
                                      reinterpret_cast<const GLfloat *>(&arr[0]));
               });
}

void ShaderProgram::setUniform(const std::string &name, const std::function<void(int)> setter)
{
    GLuint location;
    auto possibleLocation = _uniformLocations.find(name);
    if (possibleLocation != _uniformLocations.end())
    {
        location = possibleLocation->second;
    }
    else
    {
        location = glGetUniformLocation(_id, name.c_str());
        _uniformLocations.insert(std::make_pair(name, location));
    }

    if (location != -1)
    {
        setter(location);
    }
}

} // namespace graphics