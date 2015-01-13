#pragma once

#include <gob/Buffer.h>
#include <gob/BmFileData.h>

namespace Df
{

/**
    Bitmap data.
 
    Holds uncompressed bitmap data.
*/

class Bitmap :
    public IReadableBuffer
{
public:
using T = uint8_t;

    Bitmap(unsigned width, unsigned height, BmFileTransparency transparency, std::unique_ptr<IBuffer>&& buffer) :
        m_width(width),
        m_height(height),
        m_transparency(transparency),
        m_data(std::move(buffer))
    { }

    Bitmap(unsigned width, unsigned height, BmFileTransparency transparency, Buffer&& buffer) :
        Bitmap(width, height, transparency, std::make_unique<Buffer>(std::move(buffer)))
    { }
    
    /**
        Does the bitmap have valid data.
        
        A valid bitmap allows reading from the entire buffer.
    */
    bool IsValid() const
    {
        return (GetWidth() * GetHeight()) <= GetDataSize();
    }
    
    /**
        Get the width of the image.
    */
    unsigned GetWidth() const { return m_width; }
    
    /**
        Get the height of the image.
    */
    unsigned GetHeight() const { return m_height; }
    
    /**
        Get the type of transparency that should be used to render the image.
    */
    BmFileTransparency GetTransparency() const { return m_transparency; }

// IBuffer
    virtual bool IsReadable() const override { return (m_data && m_data->IsReadable()); }
    
    virtual size_t GetDataSize() const override { return m_data->GetDataSize() / sizeof(T); }
    
    virtual const T* GetR(size_t offset) const override { return m_data->GetR(offset); }
    
private:
    std::unique_ptr<IReadableBuffer> m_data;
    unsigned m_width;
    unsigned m_height;
    BmFileTransparency m_transparency;
};

} // Df