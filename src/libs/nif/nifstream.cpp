#include "nifstream.hpp"

namespace nif
{

template <std::size_t numItems, typename T>
inline void readBufferOfType(std::unique_ptr<std::istream> &input, T *dest)
{
    input->read((char *)dest, numItems * sizeof(T));
    if (input->bad())
        throw std::runtime_error("Failed to read buffer of type " + std::string(typeid(T).name()) +
                                 " of " + std::to_string(numItems) + " items");
}

template <typename T>
inline void readDynamicBufferOfType(std::unique_ptr<std::istream> &input, T *dest,
                                    std::size_t numItems)
{
    input->read((char *)dest, numItems * sizeof(T));
    if (input->bad())
        throw std::runtime_error("Failed to read dynamic buffer of type " +
                                 std::string(typeid(T).name()) + " of " + std::to_string(numItems) +
                                 " items");
}

template <typename T> T inline readType(std::unique_ptr<std::istream> &input)
{
    T val;
    readBufferOfType<1, T>(input, &val);
    return val;
}

const NifReader &NifStream::getFile() const
{
    return m_File;
}

void NifStream::skip(size_t size)
{
    m_Input->ignore(size);
}

char NifStream::getChar()
{
    return readType<char>(m_Input);
}

int8_t NifStream::getInt8()
{
    return readType<int8_t>(m_Input);
}

uint8_t NifStream::getUint8()
{
    return readType<uint8_t>(m_Input);
}

int16_t NifStream::getInt16()
{
    return readType<int16_t>(m_Input);
}

uint16_t NifStream::getUint16()
{
    return readType<uint16_t>(m_Input);
}

int32_t NifStream::getInt32()
{
    return readType<int32_t>(m_Input);
}

uint32_t NifStream::getUint32()
{
    return readType<uint32_t>(m_Input);
}

int64_t NifStream::getInt64()
{
    return readType<int16_t>(m_Input);
}

uint64_t NifStream::getUint64()
{
    return readType<uint16_t>(m_Input);
}

float NifStream::getFloat()
{
    return readType<float>(m_Input);
}

double NifStream::getDouble()
{
    return readType<double>(m_Input);
}

glm::vec2 NifStream::getVector2()
{
    float val[2];
    readBufferOfType<2, float>(m_Input, val);
    return glm::vec2(val[0], val[1]);
}

glm::vec3 NifStream::getVector3()
{
    float val[3];
    readBufferOfType<3, float>(m_Input, val);
    return glm::vec3(val[0], val[1], val[2]);
}

glm::vec4 NifStream::getVector4()
{
    float val[4];
    readBufferOfType<4, float>(m_Input, val);
    return glm::vec4(val[0], val[1], val[2], val[3]);
}

ByteColor3 NifStream::getByteColor3()
{
    uint8_t val[3];
    readBufferOfType<3, uint8_t>(m_Input, val);
    return ByteColor3(val[0], val[1], val[2]);
}

ByteColor4 NifStream::getByteColor4()
{
    uint8_t val[4];
    readBufferOfType<4, uint8_t>(m_Input, val);
    return ByteColor4(val[0], val[1], val[2], val[3]);
}

glm::mat3x3 NifStream::getMatrix33()
{
    float val[9];
    readBufferOfType<9, float>(m_Input, val);
    return glm::mat3x3(val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7], val[8]);
}

bool NifStream::getBoolean8()
{
    return getUint8() != 0;
}

bool NifStream::getBoolean32()
{
    return getUint32() != 0;
}

std::string NifStream::getString()
{
    size_t length = getUint32();
    std::string str(length, '\0');
    m_Input->read((char *)str.data(), length);
    if (m_Input->bad())
        throw std::runtime_error("Failed to read string of " + std::to_string(length) + " chars");
    size_t end = str.find('\0');
    if (end != std::string::npos)
        str.erase(end);
    return str;
}

std::string NifStream::getVersionString()
{
    std::string result;
    // Input string ends with
    std::getline(*m_Input, result);
    if (m_Input->bad())
        throw std::runtime_error("Failed to read version string");
    return result;
}

void NifStream::getChars(std::vector<char> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<char>(m_Input, vec.data(), size);
}

void NifStream::getInt8s(std::vector<int8_t> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<int8_t>(m_Input, vec.data(), size);
}

void NifStream::getUint8s(std::vector<uint8_t> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<uint8_t>(m_Input, vec.data(), size);
}

void NifStream::getInt16s(std::vector<int16_t> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<int16_t>(m_Input, vec.data(), size);
}

void NifStream::getUint16s(std::vector<uint16_t> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<uint16_t>(m_Input, vec.data(), size);
}

void NifStream::getInt32s(std::vector<int32_t> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<int32_t>(m_Input, vec.data(), size);
}

void NifStream::getUint32s(std::vector<uint32_t> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<uint32_t>(m_Input, vec.data(), size);
}

void NifStream::getInt64s(std::vector<int64_t> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<int64_t>(m_Input, vec.data(), size);
}

void NifStream::getUint64s(std::vector<uint64_t> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<uint64_t>(m_Input, vec.data(), size);
}

void NifStream::getFloats(std::vector<float> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<float>(m_Input, vec.data(), size);
}

void NifStream::getDoubles(std::vector<double> &vec, size_t size)
{
    vec.resize(size);
    readDynamicBufferOfType<double>(m_Input, vec.data(), size);
}

void NifStream::getVector2s(std::vector<glm::vec2> &vec, size_t size)
{
    vec.resize(size);
    for (size_t i = 0; i < size; i++)
    {
        vec[i] = this->getVector2();
    }
}

void NifStream::getVector3s(std::vector<glm::vec3> &vec, size_t size)
{
    vec.resize(size);
    for (size_t i = 0; i < size; i++)
    {
        vec[i] = this->getVector3();
    }
}

void NifStream::getVector4s(std::vector<glm::vec4> &vec, size_t size)
{
    vec.resize(size);
    for (size_t i = 0; i < size; i++)
    {
        vec[i] = this->getVector4();
    }
}

void NifStream::getStrings(std::vector<std::string> &vec, size_t size)
{
    vec.resize(size);
    for (size_t i = 0; i < size; i++)
    {
        vec[i] = this->getString();
    }
}

} // namespace nif
