#pragma once

#include <gob/Buffer.h>
#include <gob/DataReader.h>
#include <gob/CmpFileData.h>

namespace Df
{

/**
    Color mapping defintion file.
*/
class CmpFile :
    public IDataReader
{
public:
    /**
        Create a Cmp file from data in a file stream.
    */
    static CmpFile CreateFromDataProvider(const IDataReader& dataProvider)
    {
        return CmpFile(Buffer::CreateFromDataProvider(dataProvider));
    }
    
    CmpFile() { }
    
    CmpFile(Buffer&& buffer) :
        m_data(std::make_shared<Buffer>(std::move(buffer)))
    { }
    
    virtual size_t GetDataSize() const override
    {
        return sizeof(CmpFileData);
    }
    
    virtual bool IsReadable() const override
    {
        return (m_data && m_data->IsReadable());
    }
    
    virtual size_t Read(uint8_t* output, size_t offset, size_t max) const override
    {
        if (m_data)
            return m_data->Read(output, offset, max);
        else
            return 0;
    }

private:
    std::shared_ptr<IBuffer> m_data;
};

} // Df

