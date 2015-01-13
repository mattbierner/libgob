#pragma once

#include <stdint.h>
#include <vector>

#include <gob/DataReader.h>

namespace Df
{

/**
    Provides basic read access to a block of memory.
*/
class IReadableBuffer :
    public IDataReader
{
public:
    /**
        Get direct access to memory in the buffer.
        
        No ownership of the memory is implied. Ptr is invalid once Buffer is 
        destroyed.
        
        @param offset Absolute byte offset in buffer.
        
        Returns nullptr if invalid offset.
    */
    virtual const uint8_t* GetR(size_t offset) const = 0;
    
    /**
        Get direct access to an object of type `T` in the buffer.
        
        @param offset Absolute byte offset in buffer.
        
        Returns nullptr if invalid offset or there is not enough space to read
        an object of type `T`.
    */
    template <typename T = uint8_t>
    const T* GetObjR(size_t offset = 0) const
    {
        if (CanRead<T>(offset))
            return reinterpret_cast<const T*>(GetR(offset));
        else
            return nullptr;
    }
    
    /**
        Get reference to element at `offset`.
    */
    const uint8_t& AtR(size_t offset) const { return *GetR(offset); }
    
    /**
        Turn an absolute offset into a relative offset.
        
        This is useful for working with pointer like data members.
    */
    virtual size_t ResolveOffset(size_t offset) const
    {
        return offset;
    }
    
    virtual size_t Read(uint8_t* output, size_t offset, size_t max) const override
    {
        if (!IsReadable()) return 0;
        size_t read = std::min(GetDataSize() - offset, max);
        const uint8_t* start = GetR(offset);
        std::copy(start, start + read, output);
        return read;
    }
};

/**
    Provides basic read and write access to a block of memory.
*/
class IBuffer :
    public IReadableBuffer
{
public:
    virtual uint8_t* GetW(size_t offset)
    {
        return const_cast<uint8_t*>(static_cast<const IBuffer&>(*this).GetR(offset));
    }
    
    template <typename T = uint8_t>
    T* GetObjW(size_t offset = 0)
    {
        return const_cast<T*>(static_cast<const IBuffer&>(*this).GetObjR<T>(offset));
    }
    
    uint8_t& AtW(size_t offset) { return *GetW(offset); }
};

/**
    Managed block of memory.
*/
class Buffer : private std::vector<uint8_t>,
    public IBuffer
{
    using Super = std::vector<uint8_t>;
    
public:
    /**
        Create a buffer from a data provider.
        
        Note that this always copies the data held by the Data provider.
    */
    static Buffer CreateFromDataProvider(const IDataReader& provider)
    {
        size_t size = provider.GetDataSize();
        Buffer buf = Create(size);
        provider.Read(buf.GetW(0), 0, size);
        return buf;
    }
    
    /**
        Create a new buffer of `size`.
    */
    static Buffer Create(size_t size)
    {
        return Buffer(size);
    }
    
    Buffer() : Buffer(0) { }
    
    Buffer(const Buffer& other) = delete;
    
    Buffer(Buffer&& other) :
        Super(std::move(other))
    { }
    
    Buffer& operator=(const Buffer& other) = delete;
    
    Buffer& operator=(Buffer&& other)
    {
        Super::operator=(std::move(other));
        return *this;
    }

    virtual bool IsReadable() const override { return (GetDataSize() > 0); }

    virtual size_t GetDataSize() const override { return this->size(); }

    virtual const uint8_t* GetR(size_t offset) const override { return &(Super::operator[](offset)); }

private:
    Buffer(size_t size) :
        Super(size)
    { }
};

/**
    Buffer pointing to data inside of another buffer.
*/
class RelativeOffsetBuffer : public IBuffer
{
public:
    RelativeOffsetBuffer(const std::shared_ptr<IBuffer>& data, size_t offset, size_t size) :
        m_data(data),
        m_offset(offset),
        m_size(size)
    { }
    
     RelativeOffsetBuffer(const std::shared_ptr<IBuffer>& data, size_t offset) :
        RelativeOffsetBuffer(
            data,
            offset,
            (data ? data->GetDataSize() - offset : 0))
    { }

    virtual bool IsReadable() const override { return (m_data && (m_data->GetDataSize() - m_offset >= m_size)); }
   
    virtual size_t GetDataSize() const override
    {
        if (IsReadable())
            return m_size;
        else
            return 0;
    }
    
    virtual const uint8_t* GetR(size_t offset) const override { return m_data->GetR(m_offset + offset); }
    
    virtual size_t ResolveOffset(size_t offset) const override
    {
        return m_data->ResolveOffset(offset - m_offset);
    }

private:
    std::shared_ptr<IBuffer> m_data;
    size_t m_size;
    size_t m_offset;
};


} // Df