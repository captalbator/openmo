#pragma once

namespace alive
{

struct XrgFile
{
  std::filesystem::path path;

  std::string name;
  std::string loadingScreen;
  std::string regionSaveThumbnail;
  std::string levelMusic;

  std::string type;
  std::string audioEffect;

  std::string xinPath;

  explicit XrgFile(std::filesystem::path path) : path(path) {}
  ~XrgFile() {}

  void parse();
};

} // namespace alive
