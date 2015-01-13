#pragma once

#include <gob/Buffer.h>
#include <gob/PalFileData.h>

namespace Df
{

/**
*/
class Pal
{
public:
    Pal(const std::shared_ptr<IBuffer>& data) :
        m_data(data)
    { }

    /**
    */
    bool IsValid() const
    {
        return (m_data && m_data.GetObj<PalFileData>(0));
    }
    
    /**
     
    */
    PalFileColor operator[](size_t index) const
    {
        if (index < 256 && IsValid())
            return *m_data.GetObj<PalFileColor>(index * sizeof(PalFileColor));
        else
            return { };
    }
    
private:
    std::unique_ptr<IBuffer> m_data;
};

} // Df