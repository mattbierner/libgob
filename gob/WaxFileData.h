/**
    Structures for reading a WAX from binary data.
    
    A Wax file is a collection of animated actions. Each action is a collection
    of sequences from different angles. Each Sequence is a collection of
    animation frames.
*/
#pragma once

#include <stdint.h>

#include <gob/Common.h>

namespace Df
{

/**
    Wax file header.
*/
PACKED(struct WaxFileHeader
{
    /** Constant, file version. */
    uint32_t version;
    
    /** Number of sequences. */
    uint32_t nSeqs;
    
    /** Number of frames. */
    uint32_t nFrames;
    
    /** Number of cells. */
    uint32_t nCells;
    
    /** unused. */
    uint32_t xScale;
    
    /** unused. */
    uint32_t yScale;
    
    /** unused. */
    uint32_t xtraLight;
    
    /** unused. */
    uint32_t pad4;
    
    /** `WaxFileActionEntry` pointers. */
    uint32_t waxes[32];
});

/**
    Wax entry.
    
    An action stores views of an animations for some action, e.g. walking, attacking, ...
*/
PACKED(struct WaxFileActionEntry
{
    /** World width. */
    uint32_t worldWidth;
    
    /** World height. */
    uint32_t worldHeight;
    
    /** Frames per second. */
    uint32_t frameRate;
    
    /** unused. */
    uint32_t padding[4];
    
    /** `WaxFileSequenceEntry` pointers. */
    uint32_t sequences[32];
});

/**
    Wax sequence entry.
    
    Sequences store views from different angles.
*/
PACKED(struct WaxFileSequenceEntry
{
    /** unused. */
    uint32_t padding[4];
    
    /** Pointers to frames. Frames are stored in `FmeHeaders`. */
    uint32_t frames[32];
});

} // Df