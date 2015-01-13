#include "CompressedBuffer.h"

namespace Df
{

/*static*/ size_t CompressedBufferReader::ReadCompressedData(
    const IBuffer& buffer,
    BmFileCompression compression,
    uint8_t* output,
    size_t offset,
    size_t max)
{
    switch (compression)
    {
    case BmFileCompression::Rle:
        return ReadRleCompressedData(buffer, offset, BufferWriter(output, max)) - output;

    case BmFileCompression::Rle0:
        return ReadRle0CompressedData(buffer, offset, BufferWriter(output, max)) - output;

    case BmFileCompression::None:
    default:
        return buffer.Read(output, offset, max);
    }
}

/*static*/ BufferWriter CompressedBufferReader::ReadRleCompressedData(
    const IBuffer& buffer,
    size_t offset,
    BufferWriter writer)
{
    const uint8_t* start = buffer.GetR(offset);
    while (writer.IsWritable())
    {
        uint8_t n = *(start++);
        if (n <= 128)
        {
            // copy `n` direct values
            writer = writer.Write(start, n);
            start += n;
        }
        else
        {
            // Repeat next byte `n - 128` times
            auto next = *(start++);
            uint8_t reps = n - 128;
            writer = writer.Write([next, reps](uint8_t* data, size_t max)
            {
                size_t toRead = std::min(static_cast<size_t>(reps), max);
                std::fill_n(data, toRead, next);
                return toRead;
            });
        }
    }
    return writer;
}

/*static*/ BufferWriter CompressedBufferReader::ReadRle0CompressedData(
    const IBuffer& buffer,
    size_t offset,
    BufferWriter writer)
{
    const uint8_t* start = buffer.GetR(offset);
    while (writer.IsWritable())
    {
        uint8_t n = *(start++);
        if (n <= 128)
        {
            // copy `n` direct values
            writer = writer.Write(start, n);
            start += n;
        }
        else
        {
            // Create `n - 128` transparent pixels
            uint8_t reps = n - 128;
            writer = writer.Write([reps](uint8_t* data, size_t max)
            {
                size_t toRead = std::min(static_cast<size_t>(reps), max);
                std::fill_n(data, toRead, 0);
                return toRead;
            });
        }
    }
    return writer;
}


} // Df