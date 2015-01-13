#include "FmeFile.h"

#include "CompressedBuffer.h"

namespace Df
{

/*static*/ FmeFileHeader FmeFile::s_emptyHeader = { };
/*static*/ FmeFileHeader2 FmeFile::s_emptyHeader2 = { };

const FmeFileHeader* FmeFile::GetHeader() const
{
    if (m_data)
    {
        const auto* header = m_data->GetObjR<FmeFileHeader>(0);
        if (header)
            return header;
    }
    return &s_emptyHeader;
}

const FmeFileHeader2* FmeFile::GetHeader2() const
{
    auto* header = GetHeader();
    if (m_data && header->header2)
    {
        const auto* header2 = m_data->GetObjR<FmeFileHeader2>(m_data->ResolveOffset(header->header2));
        if (header2)
            return header2;
    }
    return &s_emptyHeader2;
}

Buffer FmeFile::Uncompress() const
{
    size_t size = GetDataSize();
    Df::Buffer data = Df::Buffer::Create(size);

    CompressedBufferReader::ReadCompressedData(
        *m_data,
        GetCompression(),
        data.GetW(0),
        GetImageDataStart(),
        size);
    return data;
}

Bitmap FmeFile::CreateBitmap() const
{
    unsigned width = GetWidth();
    unsigned height = GetHeight();
    BmFileTransparency transparent = BmFileTransparency::Transparent;
    if (IsCompressed())
    {
        // Uncompress data.
        return Bitmap(width, height, transparent, Uncompress());
    }
    else
    {
        return Bitmap(
            width,
            height,
            transparent,
            std::make_unique<RelativeOffsetBuffer>(m_data, GetImageDataStart(), GetDataSize()));
    }
}

const size_t FmeFile::GetImageDataStart() const
{
    size_t header2Offset = m_data->ResolveOffset(GetHeader()->header2);
    size_t dataOffset = header2Offset + sizeof(FmeFileHeader2);
    if (IsCompressed())
    {
        size_t imgDataOffset = dataOffset + sizeof(uint32_t) * GetWidth();
        return imgDataOffset;
    }
    else
    {
        return dataOffset;
    }
}

} // Df
