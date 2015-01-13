#pragma once

#include <stdint.h>

namespace Df
{

class BufferWriter
{
public:
    BufferWriter(uint8_t* buffer, size_t size) :
        m_buffer(buffer),
        m_size(size)
    { }

    size_t operator-(const uint8_t* ptr) const { return (m_buffer - ptr); }

    virtual bool IsWritable() const
    {
        return (m_buffer != nullptr && m_size > 0);
    }

    /**
        Get total data size.
        
        Returns `0` if not valid.
    */
    size_t GetDataSize() const
    {
        return (IsWritable() ? m_size : 0);
    }
    
    /**
    */
    BufferWriter Write(const uint8_t* data, size_t max)
    {
        return Write([data, max](uint8_t* buffer, size_t maxToWrite)
        {
            size_t written = std::min(maxToWrite, max);
            std::copy(data, data + written, buffer);
            return written;
        });
    }
    
    /**
    */
    template <typename F>
    BufferWriter Write(const F& f)
    {
        if (!IsWritable()) return *this;
        size_t written = f(Get(), GetDataSize());
        return BufferWriter(m_buffer + written, m_size - written);
    }
    
private:
    size_t m_size;
    uint8_t* m_buffer;
    
    
    uint8_t* Get() { return m_buffer; }

};

} // Df