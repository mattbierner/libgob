/**
    Structures for reading a CMP (color light level mapping) from binary data.
*/
#pragma once

#include <stdint.h>

#include <gob/Common.h>

namespace Df
{

/**
    Mapping for one light level in the colormap.
*/
PACKED(struct CmpFileColorMap
{
    /** Indicies into a PAL file. */
    uint8_t colors[256];
});

/**
    Complete CMP file.
*/
PACKED(struct CmpFileData
{
    /** Collection of color mappings for the 32 different light levels. */
    CmpFileColorMap colorMaps[32];
    
    /** Additional shading used for the headlight and weapon lighting effects. */
    uint8_t headlightMap[128];
});

} // Df