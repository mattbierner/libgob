#pragma once

#include "Buffer.h"

#include "BmFileData.h"
#include "BufferWriter.h"

namespace Df
{

class CompressedBufferReader
{
public:
    static size_t ReadCompressedData(
        const IBuffer& buffer,
        BmFileCompression compression,
        uint8_t* output,
        size_t offset,
        size_t max);

private:
    static BufferWriter ReadRleCompressedData(const IBuffer& buffer, size_t offset, BufferWriter writer);
    
    static BufferWriter ReadRle0CompressedData(const IBuffer& buffer, size_t offset, BufferWriter writer);
};

} // Df