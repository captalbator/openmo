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
  std::filesystem::path _path;
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

  void setUniform(const std::string &name, int value);
  void setUniform(const std::string &name, unsigned int value);
  void setUniform(const std::string &name, bool value);
  void setUniform(const std::string &name, float value);
  void setUniform(const std::string &name, const glm::vec2 &value);
  void setUniform(const std::string &name, const glm::vec3 &value);
  void setUniform(const std::string &name, const glm::mat4 &value);
  void setUniform(const std::string &name, const std::vector<glm::mat4> &arr);
  void setUniform(const std::string &name, const std::function<void(int)> setter);

private:
  GLuint _id{0};
  std::map<std::string, int> _uniformLocations;
};

} // namespace graphics
