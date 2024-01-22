#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base.hpp"

namespace nif
{

struct NifFile
{
    //~NifFile() {}

    uint32_t version = 0;

    std::filesystem::path path;

    std::vector<std::unique_ptr<NiObject>> blocks;
    std::vector<NiObject *> roots;

    explicit NifFile(const std::filesystem::path &path) : path(path) {}
};

class NifReader
{
public:
    explicit NifReader(NifFile &file);

    void parse(std::unique_ptr<std::istream> stream);

    NiObject *getBlock(size_t index) const { return m_Blocks.at(index).get(); }

private:
    uint32_t &m_Version;

    std::filesystem::path &m_Path;

    std::vector<std::unique_ptr<NiObject>> &m_Blocks;
    std::vector<NiObject *> &m_Roots;
};

} // namespace nif
