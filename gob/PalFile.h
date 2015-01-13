#pragma once

#include <gob/Buffer.h>
#include <gob/DataReader.h>
#include <gob/PalFileData.h>

namespace Df
{

/**
    Color palette defintion file.
*/
class PalFile :
    public IDataReader
{
public:
    /**
        Create a Pal file from data in a file stream.
    */
    static PalFile CreateFromDataProvider(const IDataReader& dataProvider)
    {
        return PalFile(Buffer::CreateFromDataProvider(dataProvider));
    }
    
    PalFile() { }
    
    PalFile(Buffer&& buffer) :
        m_data(std::make_shared<Buffer>(std::move(buffer)))
    { }
    
    virtual size_t GetDataSize() const override
    {
        return sizeof(PalFileData);
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

