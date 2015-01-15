#pragma once

#include <gob/Buffer.h>
#include <gob/CmpFile.h>
#include <gob/CmpFileData.h>

namespace Df
{

/**
*/
class Cmp
{
public:
 /**
        Create a Cmp from a Fme file.
    */
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
    */
    bool IsValid() const
    {
        return (m_data && m_data->GetDataSize() >= sizeof(CmpFileData));
    }
    
    const uint8_t* GetData() const
    {
         if (IsValid())
            return m_data->GetObjR<uint8_t>(0);
        else
            return nullptr;
    }

    uint8_t GetShading(size_t colorMap, size_t index) const
    {
        if (index < 256 && colorMap < 32 && IsValid())
        {
            return *m_data->GetObjR<uint8_t>((colorMap * sizeof(CmpFileColorMap)) + index);
        }
        
        return { };
    }
    
private:
    std::unique_ptr<IBuffer> m_data;
};

} // Df