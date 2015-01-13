#include "BmFile.h"

#include "CompressedBuffer.h"

#include <cassert>

namespace Df
{

/*static*/ BmFileHeader BmFile::s_emptyHeader = { };
/*static*/ BmFileSubHeader BmFile::s_emptySubHeader = { };

const BmFileHeader* BmFile::GetHeader() const
{
    if (m_data)
    {
        const auto* header = m_data->GetObjR<BmFileHeader>(0);
        if (header)
            return header;
    }
    return &s_emptyHeader;
}

uint8_t BmFile::GetFrameRate() const
{
    if (IsMultipleBm())
    {
        const uint8_t* frameRate = m_data->GetR(sizeof(BmFileHeader));
        if (frameRate)
            return *frameRate;
    }
    
    return 0;
}

const BmFileSubHeader* BmFile::GetSubHeader(size_t index) const
{
    assert(IsMultipleBm() && index < GetCountSubBms());
    if (m_data)
    {
        int32_t offset = GetSubOffset(index);
        const auto* subHeader = m_data->GetObjR<BmFileSubHeader>(offset);
        if (subHeader)
            return subHeader;
    }
    return &s_emptySubHeader;
}

Buffer BmFile::Uncompress(size_t index) const
{
    size_t size = GetDataSize(index);
    Df::Buffer data = Df::Buffer::Create(size);

    CompressedBufferReader::ReadCompressedData(
        *m_data,
        GetCompression(),
        data.GetW(0),
        GetImageDataStart(index),
        size);
    return data;
}

Bitmap BmFile::CreateBitmap(size_t index) const
{
    unsigned width = GetWidth(index);
    unsigned height = GetHeight(index);
    BmFileTransparency transparency = GetTransparency();
    
    if (IsCompressed())
    {
        return Bitmap(width, height, transparency, Uncompress(index));
    }
    else
    {
        return Bitmap(
            width,
            height,
            transparency,
            std::make_unique<RelativeOffsetBuffer>(m_data, GetImageDataStart(index), GetDataSize(index)));
    }
}

int32_t BmFile::GetSubOffset(size_t index) const
{
    assert(IsMultipleBm() && index < GetCountSubBms());

    const uint32_t* startTable = m_data->GetObjR<uint32_t>(sizeof(BmFileHeader) + 2);
    if (startTable)
    {
        return startTable[index] + sizeof(BmFileHeader) + 2;
    }
    else
    {
        return 0;
    }
}

const size_t BmFile::GetImageDataStart(size_t index) const
{
    if (IsCompressed())
    {
        size_t dataOffset = sizeof(BmFileHeader);
        size_t tableOffset = dataOffset + GetHeader()->dataSize;
        const uint32_t* tableEntry = m_data->GetObjR<uint32_t>(tableOffset);
        if (tableEntry)
            return dataOffset + *tableEntry;
        else
            return 0;
    }
    else
    {
        return (IsMultipleBm() ? GetSubOffset(index) + sizeof(BmFileSubHeader) : sizeof(BmFileHeader));
    }
}

} // Df
