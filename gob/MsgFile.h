#pragma once

#include <gob/Buffer.h>
#include <gob/Msg.h>

namespace Df
{

class MsgFile
{
public:
    /**
        Create a BM file from data in a file stream.
    */
    static MsgFile CreateFromDataProvider(const IDataReader& dataProvider)
    {
        return MsgFile(Buffer::CreateFromDataProvider(dataProvider));
    }
    
    MsgFile(Buffer&& data) :
        m_data(std::move(data))
    { }
    
    /**
    */
    Msg CreateMsg() const;
    
private:
    Buffer m_data;
};

} // Df