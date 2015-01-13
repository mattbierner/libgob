#pragma once

#include <gob/Buffer.h>
#include <gob/Tdo.h>

namespace Df
{

class TdoFile
{
public:
    TdoFile(Buffer&& data) :
        m_data(std::move(data))
    { }
    
    Tdo CreateTdo() const;
    
    
private:
    Buffer m_data;
};

} // Df