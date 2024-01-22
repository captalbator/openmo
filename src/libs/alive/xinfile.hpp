#pragma once

namespace alive
{

struct LayoutInfo
{
    std::string guid;
    std::string author;
    std::string creationDate;
    std::string tagTemplateFilename;
};

struct ZoneInfo
{
    std::string zoneName = "HappyZone";

    int startGuid = 0;
    int nextGuid = 0;
    int endGuid = 9999;

    std::string envTheme = "native";
    std::string envType = "exterior";
    std::string path;

    std::filesystem::path sceneGraph;
    std::filesystem::path tagTemplateFile;
};

struct ZoneSettings
{
    glm::vec3 bgColor = {0.f, 0.f, 0.f};

    bool fog = true;
    glm::vec3 fogColor = {0.f, 0.f, 0.f};
    float fogDepth = 100.f;
    float fogMax = 1.f;
    std::string fogFunction = "squared range";

    bool dithering = false;

    bool sky = false;
    std::string skyModel = "";

    float fallingLimit = -200.f;
    float health = 0.f;
    bool allowRespawn = false;
};

struct XinFile
{
    std::filesystem::path path;

    LayoutInfo layoutInfo;
    ZoneInfo zoneInfo;
    ZoneSettings zoneSettings;

    std::vector<std::filesystem::path> includes;

    std::filesystem::path templateFile;
    // TODO: tag list

    explicit XinFile(std::filesystem::path path) : path(path) {}

    ~XinFile() {}

    void parse();
};

} // namespace alive