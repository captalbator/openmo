#pragma once

#include "nifstream.hpp"

namespace nif
{

template <class X> class NiBlockPtr
{
public:
    NiBlockPtr() : m_Index(-2) {}
    // NiBlockPtr(intptr_t index) : index(index) {}
    NiBlockPtr(X *ptr) : m_Ptr(ptr) {}

    void read(NifStream *nif)
    {
        assert(m_Index == -2);
        m_Index = nif->getInt32();
        assert(m_Index >= -1);
    }

    void post(NifReader &reader)
    {
        if (m_Index < 0)
        {
            m_Ptr = nullptr;
        }
        else
        {
            // get pointer from index
            NiObject *b = reader.getBlock(m_Index);
            m_Ptr = dynamic_cast<X *>(b);
            assert(m_Ptr != nullptr);
        }
    }

    intptr_t index() const { return m_Index; }
    X *ptr() const { return m_Ptr; }

    bool empty() const { return m_Ptr == nullptr; }

private:
    union
    {
        intptr_t m_Index;
        X *m_Ptr;
    };
};

template <class X> using NiBlockList = std::vector<NiBlockPtr<X>>;

template <class T> void readBlockList(NifStream *nif, NiBlockList<T> &list)
{
    const int length = nif->getInt32();

    if (length < 0)
        throw std::runtime_error("Negative NIF block list length: " + std::to_string(length));

    list.resize(static_cast<std::size_t>(length));

    for (auto &value : list)
        value.read(nif);
}

template <class T> void postBlockList(NifReader &nif, NiBlockList<T> &list)
{
    for (auto &value : list)
        value.post(nif);
}

} // namespace nif
