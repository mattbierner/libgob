#pragma once

#include <gob/Buffer.h>
#include <gob/Gol.h>

namespace Df
{

class GolFile
{
public:
    GolFile(Buffer&& data) :
        m_data(std::move(data))
    { }
    
    Gol CreateGol() const;
    
private:
    Buffer m_data;
};

} // Df

