/**
    Structures for reading a BM from binary data.
*/
#pragma once

#include <stdint.h>

#include <gob/Common.h>

namespace Df
{

/**
    Type of transparency used in a BM file.
*/
enum class BmFileTransparency : uint8_t
{
    Normal = 0x36,
    Transparent = 0x3e,
    Weapon = 0x08
};

/**
    Type of compression used in a BM file.
*/
enum class BmFileCompression : uint16_t
{
    None = 0,
    Rle = 1,
    Rle0 = 2
};

/**
    File header.
*/
PACKED(struct BmFileHeader
{
    /** GOB header. */
    char magic[4];
    
    int16_t sizeX;
    int16_t sizeY;
    
    /** Unused by engine. */
    int16_t idemX;
    
    /** Number of sub BMs in a multuple BM. */
    int16_t idemY;
    
    /** Type of transparency. */
    BmFileTransparency transparency;
    
    /** log2 of `sizeY`. */
    uint8_t logSizeY;
    
    /** Type of compression. */
    BmFileCompression compression;
    
    /** Size of data, excluding header, for compressed BM. */
    int32_t dataSize;
    
    uint8_t padding[12];
});

/**
    Header for a sub BM file in a multiple BM.
*/
PACKED(struct BmFileSubHeader
{
    /** Vertical size. */
    int16_t sizeX;
    
    /** Horizontal size. */
    int16_t sizeY;
    
    /** Unused by engine. */
    int16_t idemX;
    int16_t idemY;
    
    /** Unused no compression */
    int32_t dataSize;
    
    uint8_t logSizeY;
    
    uint8_t pad[11];
    
    /** Type of transparency. */
    BmFileTransparency transparency;
    
    uint8_t pad2[3];
});

/**
    Immediately follows the `BmFileHeader` for a multiple BM.
*/
PACKED(struct BmFileMultipleIndex
{
    /** Frame rate of animated texture or 0 if it is a switch. */
    uint8_t frameRate;
    
    uint8_t padding;
    
    /** List of `idemY` offsets to sub BMs */
    int32_t offsets[];
});

} // Df