#pragma once

#include <gob/Buffer.h>
#include <gob/CmpFile.h>
#include <gob/CmpFileData.h>

namespace Df
{

/**
    Color map.
    
    Provides a mapping of colors at various light levels. 
*/
class Cmp
{
public:
    static Cmp CreateFromFile(const CmpFile& pal)
    {
        size_t size = pal.GetDataSize();
        auto buffer = std::make_unique<Buffer>(Buffer::Create(size));
        pal.Read(buffer->GetW(0), 0, size);

        return Cmp(std::move(buffer));
    }

    Cmp() { }

    Cmp(std::unique_ptr<IBuffer>&& data) :
        m_data(std::move(data))
    { }

    /**
        Is the Cmp valid.
        
        A valid Cmp contains 32 color maps.
    */
    bool IsValid() const
    {
        return (m_data && m_data->GetDataSize() >= sizeof(CmpFileData));
    }
    
    /**
        Access the raw cmp data.
    */
    const uint8_t* GetData() const
    {
         if (IsValid())
            return m_data->GetObjR<uint8_t>(0);
        else
            return nullptr;
    }
    
    /**
        Access the raw color map data for an individual light level.
        
        @param index Index of the color map to access.
    */
    const CmpFileColorMap* GetColorMap(size_t index) const
    {
        if (index < 32 && IsValid())
        {
            return m_data->GetObjR<CmpFileColorMap>(index * sizeof(CmpFileColorMap));
        }
        return nullptr;
    }

    /**
        Lookup the mapping of an individual color in the colormap.
        
        @param colorMap Index of the color map to access.
        @param index Index of the shading to lookup mapping for.
    */
    uint8_t GetShading(size_t colorMap, size_t index) const
    {
        auto map = GetColorMap(colorMap);
        if (map && index < 256)
            return map->colors[index];
        
        return { };
    }
    
private:
    std::unique_ptr<IBuffer> m_data;
};

} // Df