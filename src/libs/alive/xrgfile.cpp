#include "xrgfile.hpp"

#include "libs/common/log.hpp"
#include "pugixml.hpp"

namespace alive
{

void XrgFile::parse()
{
    LOG_ASSERT(path.extension() == ".xrg", "Expected .xrg file, got {}", path.string());

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    if (!result)
    {
        LOG_WARN("Failed to parse file {}", path.string());
        LOG_WARN("Error description: {}", result.description());
        LOG_WARN("Error offset: {}", result.offset);
        throw std::runtime_error("Failed to parse XML file");
    }
    else
    {
        LOG_INFO("Successfully parsed XML file \"{}\"", path.string());
    }

    pugi::xml_node root = doc.child("region");
    if (root == nullptr)
    {
        throw std::runtime_error("Couldn't find region node of file \"" + path.string() + "\"");
    }

    name = root.attribute("name").as_string();
    type = root.attribute("type").as_string();
    loadingScreen = root.attribute("loadingscreen").as_string();
    regionSaveThumbnail = root.attribute("regionsavethumbnail").as_string();
    levelMusic = root.attribute("levelmusic").as_string();
    audioEffect = root.attribute("audioeffect").as_string();

    pugi::xml_node zones = root.child("zones");
    if (zones == nullptr)
    {
        throw std::runtime_error("Region is missing zones node");
    }

    // As far as I'm aware, there's only ever 1 zone defined in region files
    pugi::xml_node zone = zones.child("zone");
    if (zone == nullptr)
    {
        throw std::runtime_error("Region is missing zones");
    }

    xinPath = zone.attribute("path").value();
}

} // namespace alive