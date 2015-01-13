#include "WaxFile.h"

#include <cassert>

template <typename T, size_t N>
constexpr bool InExtent(const T(&)[N], size_t index)
{
    return (index < N);
}

namespace Df
{

/*static*/ WaxFileSequenceEntry WaxFileSequence::s_emptyHeader = { };

size_t WaxFileSequence::GetFramesCount() const
{
    const auto* header = GetHeader();
    unsigned i = 0;
    for (; i < std::extent<decltype(header->frames)>(); ++i)
        if (header->frames[i] == 0)
            return i;
    return i;
}

FmeFile WaxFileSequence::GetFrame(size_t index) const
{
    assert(HasFrame(index));
    size_t dataOffset = GetHeader()->frames[index];
    size_t offset = dataOffset;
    return FmeFile(std::make_shared<RelativeOffsetBuffer>(m_data, offset));
}

size_t WaxFileSequence::GetDataUid(size_t index) const
{
    assert(index < GetFramesCount());
    return GetHeader()->frames[index];
}

const WaxFileSequenceEntry* WaxFileSequence::GetHeader() const
{
   if (m_data)
    {
        const auto* header = m_data->GetObjR<WaxFileSequenceEntry>(m_offset);
        if (header)
            return header;
    }
    return &s_emptyHeader;
}

/*static*/ WaxFileActionEntry WaxFileAction::s_emptyHeader = { };

WaxFileSequence WaxFileAction::GetSequence(size_t index) const
{
    assert(index < GetSequencesCount());
    const auto* header = GetHeader();
    size_t offset = header->sequences[index];
    
    return WaxFileSequence(m_data, offset);
}

const WaxFileActionEntry* WaxFileAction::GetHeader() const
{
    if (m_data)
    {
        const auto* header = m_data->GetObjR<WaxFileActionEntry>(m_offset);
        if (header)
            return header;
    }
    return &s_emptyHeader;
}

/*static*/ WaxFileHeader WaxFile::s_emptyHeader = { };

std::vector<size_t> WaxFile::GetActions() const
{
    const auto* header = GetHeader();
    
    std::vector<size_t> indicies;
    for (unsigned i = 0; i < std::extent<decltype(header->waxes)>(); ++i)
        if (header->waxes[i])
            indicies.push_back(i);
    return indicies;
}

bool WaxFile::HasAction(size_t index) const
{
    const auto* header = GetHeader();
    return (InExtent(header->waxes, index) && header->waxes[index] > 0);
}

WaxFileAction WaxFile::GetAction(size_t index) const
{
    assert(HasAction(index));
    const auto* header = GetHeader();
    size_t offset = header->waxes[index];
    
    return WaxFileAction(m_data, offset);
}

const WaxFileHeader* WaxFile::GetHeader() const
{
    if (m_data)
    {
        const auto* header = m_data->GetObjR<WaxFileHeader>(0);
        if (header)
            return header;
    }
    return &s_emptyHeader;
}

} // Df
