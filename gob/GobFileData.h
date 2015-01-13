/**
    Structures for reading a GOB from binary data.
*/
#pragma once

#include <stdint.h>

#include <gob/Common.h>

namespace Df
{

/**
    File header.
*/
PACKED(struct GobFileHeader
{
    /** GOB header. */
    char magic[4];
    
    /** Absolute offset to index. */
    uint32_t indexOffset;
});

/**
    Entry for a file in the container.
*/
PACKED(struct GobFileEntry
{
    /** Absolute offset to start of file data. */
    uint32_t offset;
    
    /** Size of file data. */
    uint32_t size;
    
    /** Null terminated file name. */
    char filename[13];
});

/**
    List of files in the container.
*/
PACKED(struct GobFileIndex
{
    /** Number of files in the container. */
    uint32_t count;
    
    /** Array of `count` entries. */
    GobFileEntry entries[];
});

} // Df