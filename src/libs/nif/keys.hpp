#pragma once

#include "nifstream.hpp"

namespace nif {

enum NiKeyType : uint32_t {
    NO_INTERP,
    LINEAR_KEY,
    QUADRATIC_KEY,
    TBC_KEY,
    XYZ_ROTATION_KEY,
    CONST_KEY
};

struct NiTextKey {
    float time = 0.0f;
    std::string value;
};

struct TBC {
    float tension = 0.0f;
    float bias = 0.0f;
    float continuity = 0.0f;
};

template <typename T>
struct KeyT {
    NiKeyType type = NO_INTERP;    

    float time = 0.0f;
    T value{};
    T forward{};
    T backward{};
    TBC tbc;
};

template <typename T, T (NifStream::*getValue)()>
struct KeyMapT {
    using MapType = std::map<float, KeyT<T>>;
    using KeyType = KeyT<T>;

    uint32_t nKeys;
    NiKeyType interpolation = NO_INTERP;
    MapType keys;

    void read(NifStream* nif)
    {
        size_t count = nif->getUint32();

        if (count != 0)
            interpolation = (NiKeyType)nif->getUint32();

        KeyType key = {};

        if (interpolation == LINEAR_KEY || interpolation == CONST_KEY) {
            for (size_t i = 0; i < count; i++) {
                float t = nif->getFloat();
                key.value = (nif->*getValue)();
                keys[t] = key;
            }
        } else if (interpolation == QUADRATIC_KEY) {
            for (size_t i = 0; i < count; i++) {
                float t = nif->getFloat();
                key.value = (nif->*getValue)();
                key.forward = (nif->*getValue)();
                key.backward = (nif->*getValue)();
                keys[t] = key;
            }
        } else if (interpolation == TBC_KEY) {
            for (size_t i = 0; i < count; i++) {
                float t = nif->getFloat();
                key.value = (nif->*getValue)();
                key.tbc.tension = nif->getFloat();
                key.tbc.bias = nif->getFloat();
                key.tbc.continuity = nif->getFloat();
                keys[t] = key;
            }
        } else if (count != 0) {
            throw std::runtime_error("Unknown interp type: " + std::to_string(interpolation));
        }
    }
};

using FloatKeyMap = KeyMapT<float, &NifStream::getFloat>;
using Vector3KeyMap = KeyMapT<glm::vec3, &NifStream::getVector3>;
using Vector4KeyMap = KeyMapT<glm::vec4, &NifStream::getVector4>;

using FloatKeyMapPtr = std::shared_ptr<FloatKeyMap>;
using Vector3KeyMapPtr = std::shared_ptr<Vector3KeyMap>;
using Vector4KeyMapPtr = std::shared_ptr<Vector4KeyMap>;

} // namespace nif
