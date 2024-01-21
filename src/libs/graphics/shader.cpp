#include "shader.hpp"
#include "libs/common/log.hpp"
#include <fstream>

#include "glad/glad.h"

namespace graphics
{

std::string readEntireFile(std::filesystem::path filepath)
{
    std::string content;
    std::ifstream file_stream(filepath, std::ios::in);

    if (!file_stream.is_open())
    {
        LOG_ERROR("Could not read file {}: file does not exist", filepath.string().c_str());
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

} // namespace graphics