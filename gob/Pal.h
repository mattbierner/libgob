#pragma once

#include <gob/Buffer.h>
#include <gob/PalFile.h>
#include <gob/PalFileData.h>

namespace Df
{

/**
    Color palette file.
    
    Maps 8-bit color indicies to RGB colors.
*/
class Pal
{
public:
    static Pal CreateFromFile(const PalFile& pal)
    {
        size_t size = pal.GetDataSize();
        auto buffer = std::make_unique<Buffer>(Buffer::Create(size));
        pal.Read(buffer->GetW(0), 0, size);

        return Pal(std::move(buffer));
    }

    Pal() { }

    Pal(std::unique_ptr<IBuffer>&& data) :
        m_data(std::move(data))
    { }

    /**
        Is the Pal file valid?
        
        A valid Pal file provides 256 mappings.
    */
    bool IsValid() const
    {
        return (m_data && m_data->GetDataSize() >= sizeof(PalFileData));
    }
    
    size_t GetDataSize() const
    {
        return sizeof(PalFileData);
    }
    
    const PalFileColor* GetColors() const
    {
         if (IsValid())
            return m_data->GetObjR<PalFileColor>(0);
        else
            return nullptr;
    }
    
    /**
        Lookup the mapping of an individual color in the palette.
    */
    PalFileColor operator[](size_t index) const
    {
        if (index < 256 && IsValid())
            return *(m_data->GetObjR<PalFileColor>(index * sizeof(PalFileColor)));
        else
            return { };
    }
    
private:
    std::unique_ptr<IBuffer> m_data;
};

} // Df