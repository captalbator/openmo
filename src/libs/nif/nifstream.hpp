#pragma once

#include "nifreader.hpp"
#include "types.hpp"

namespace nif
{

class NifStream
{
public:
    NifStream(const NifReader &file, std::unique_ptr<std::istream> &&stream)
        : m_File(file), m_Input(std::move(stream))
    {
    }

    const NifReader &getFile() const;

    void skip(size_t size);

    char getChar();
    int8_t getInt8();
    uint8_t getUint8();
    int16_t getInt16();
    uint16_t getUint16();
    int32_t getInt32();
    uint32_t getUint32();
    // FIXME: to my knowledge, 64 bit types are not used in .nif files
    int64_t getInt64();
    uint64_t getUint64();

    float getFloat();
    double getDouble();

    glm::vec2 getVector2();
    glm::vec3 getVector3();
    glm::vec4 getVector4();
    ByteColor3 getByteColor3();
    ByteColor4 getByteColor4();
    glm::mat3x3 getMatrix33();

    // There are two kinds of booleans
    bool getBoolean8();
    bool getBoolean32();

    std::string getString();

    std::string getVersionString();

    void getChars(std::vector<char> &vec, size_t size);
    void getInt8s(std::vector<int8_t> &vec, size_t size);
    void getUint8s(std::vector<uint8_t> &vec, size_t size);
    void getInt16s(std::vector<int16_t> &vec, size_t size);
    void getUint16s(std::vector<uint16_t> &vec, size_t size);
    void getInt32s(std::vector<int32_t> &vec, size_t size);
    void getUint32s(std::vector<uint32_t> &vec, size_t size);
    void getInt64s(std::vector<int64_t> &vec, size_t size);
    void getUint64s(std::vector<uint64_t> &vec, size_t size);

    void getFloats(std::vector<float> &vec, size_t size);
    void getDoubles(std::vector<double> &vec, size_t size);

    void getVector2s(std::vector<glm::vec2> &vec, size_t size);
    void getVector3s(std::vector<glm::vec3> &vec, size_t size);
    void getVector4s(std::vector<glm::vec4> &vec, size_t size);

    void getStrings(std::vector<std::string> &vec, size_t size);

private:
    const NifReader &m_File;
    std::unique_ptr<std::istream> m_Input;
};

} // namespace nif
