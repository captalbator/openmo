#include "xinfile.hpp"

#include "libs/common/log.hpp"

namespace alive
{

static glm::vec3 stringToVec3(std::string str)
{
  std::string delimiter = ",";

  size_t pos = 0;
  size_t n = 0;
  std::string token;
  glm::vec3 result;
  while ((pos = str.find(delimiter)) != std::string::npos)
  {
    token = str.substr(0, pos);
    result[n] = std::stof(token);
    str.erase(0, pos + delimiter.length());
    n++;
  }
  return result;
}

void XinFile::parseLayoutInfo(pugi::xml_node &root)
{
  layoutInfo.guid = root.attribute("guid").as_string();
  layoutInfo.author = root.attribute("author").as_string();
  layoutInfo.creationDate = root.attribute("creationdate").as_string();
  layoutInfo.tagTemplateFilename = root.attribute("tagtemplatefilename").as_string();
}

void XinFile::parseZoneInfo(pugi::xml_node &zone)
{
  zoneInfo.zoneName = zone.attribute("zonename").as_string();

  zoneInfo.startGuid = zone.attribute("startguid").as_int();
  zoneInfo.nextGuid = zone.attribute("nextguid").as_int();
  zoneInfo.endGuid = zone.attribute("endguid").as_int();

  zoneInfo.envTheme = zone.attribute("envtheme").as_string();
  zoneInfo.envType = zone.attribute("envtype").as_string();
  zoneInfo.path = zone.attribute("path").as_string();

  zoneInfo.sceneGraph = zone.attribute("scenegraph").as_string();
  zoneInfo.tagTemplateFile = zone.attribute("tagtemplatefile").as_string();
}

void XinFile::parseZoneSettings(pugi::xml_node &settings)
{
  zoneSettings.bgColor = stringToVec3(settings.attribute("bgcolor").as_string());
  zoneSettings.fog = settings.attribute("fog").as_bool(true);
  zoneSettings.fogColor = stringToVec3(settings.attribute("fogcolor").as_string());
  zoneSettings.fogDepth = settings.attribute("fogdepth").as_float();
  zoneSettings.fogMax = settings.attribute("fogmax").as_float();
  zoneSettings.fogFunction = settings.attribute("fogfunction").as_string();
  zoneSettings.dithering = settings.attribute("dithering").as_bool();
  zoneSettings.sky = settings.attribute("sky").as_bool();
  zoneSettings.skyModel = settings.attribute("skymodel").as_string();
  zoneSettings.fallingLimit = settings.attribute("fallinglimit").as_float();
  zoneSettings.health = settings.attribute("health").as_float();
  zoneSettings.allowRespawn = settings.attribute("allowrespawn").as_bool();
}

void XinFile::parse()
{
  LOG_TRACE("XinFile path: {}", path.string());
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(path.c_str());
  // TODO: check result and handle accordingly

  pugi::xml_node root = doc.child("aliveLayout");
  if (root == nullptr)
  {
    throw std::runtime_error("Couldn't find root node of file \"" + path.string() + "\"");
  }

  parseLayoutInfo(root);

  pugi::xml_node zone = root.child("zone");
  if (zone == nullptr)
  {
    throw std::runtime_error("Couldn't find zone node of file \"" + path.string() + "\"");
  }

  parseZoneInfo(zone);

  pugi::xml_node settings = root.child("settings");
  if (settings == nullptr)
  {
    throw std::runtime_error("Couldn't find settings node of file \"" + path.string() + "\"");
  }

  parseZoneSettings(settings);

  pugi::xml_node includesNode = root.child("includes");

  pugi::xml_node item = includesNode.child("item");
  while (item != nullptr)
  {
    std::filesystem::path itemPath;
    itemPath = item.attribute("filename").as_string();
    includes.push_back(itemPath);

    item = includesNode.next_sibling("item");
  }

  pugi::xml_node tagListNode = root.child("taglist");

  templateFile = tagListNode.attribute("templatefile").as_string();

  pugi::xml_node currentTag = tagListNode.first_child();
  while (currentTag != nullptr)
  {
    // TODO: parse tags
    currentTag = tagListNode.next_sibling();
  }
}

} // namespace alive
