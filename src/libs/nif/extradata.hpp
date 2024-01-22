#pragma once

#include "base.hpp"
#include "blockptr.hpp"

namespace nif {

struct Extra : public NiObject {
    NiBlockPtr<Extra> next;
    size_t size;

    void read(NifStream* nif) override;
    void post(NifReader& reader) override;
};

struct NiExtraData : public Extra {
    NiBlockPtr<NiExtraData> nextExtraData;
    std::vector<uint8_t> bytes;

    void read(NifStream* nif) override;
};

struct NiStringExtraData : public Extra {
    std::string stringData;
    
    void read(NifStream* nif) override;
};

};
