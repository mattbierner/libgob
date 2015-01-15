#pragma once

#include <gob/Buffer.h>
#include <gob/PalFile.h>
#include <gob/PalFileData.h>

namespace Df
{

/**
*/
class Pal
{
public:
    /**
        Create a Pal from a Fme file.
    */
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