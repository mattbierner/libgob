#pragma once

#include <fstream>

namespace Df
{

/**
    Provides read access to memory.
*/
class IDataReader
{
public:
    virtual ~IDataReader() { }

    /**
        Can any reads be performed?
    */
    virtual bool IsReadable() const = 0;

    /**
        Get total data size.
        
        Returns `0` if not valid.
    */
    virtual size_t GetDataSize() const = 0;
    
    /**
        Copy some data.
        
        Does nothing if not valid.
     
        @param output Byte buffer output of at least `max`.
        @param offset Offset from start of memory to read at.
        @param max Maximum number of bytes to copy.
        
        @returns Size of data actually written to `output`.
    */
    virtual size_t Read(uint8_t* output, size_t offset, size_t max) const = 0;
    
    /**
        Read a single object of type `T` from the BM.
        
        Does nothing and returns zero if there is not enough space to read from.
    */
    template <typename T>
    size_t ReadObj(T* output, size_t offset) const
    {
        if (CanRead<T>(offset))
            return Read(reinterpret_cast<uint8_t*>(output), offset, sizeof(T));
        else
            return 0;
    }
    
protected:
    /**
        Can `size` bytes at `offset` be read?
    */
    bool CanRead(size_t offset, size_t size) const
    {
        return (size <= GetDataSize() - offset);
    }

    /**
        Can `sizeof(T)` byes at `offset` be read?
    */
    template <typename T>
    bool CanRead(size_t offset) const
    {
        return CanRead(offset, sizeof(T));
    }
};

/**
    Provides read access to a file stream.
*/
class FileDataProvider : public IDataReader
{
public:
    FileDataProvider(std::ifstream&& s) :
        m_stream(std::move(s)),
        m_size(0)
    {
        if (m_stream.is_open())
        {
            m_stream.seekg(0, std::ifstream::end);
            m_size = m_stream.tellg();
        }
    }
    
    virtual ~FileDataProvider() { m_stream.close(); }
    
    virtual bool IsReadable() const { return (m_size > 0);}

    virtual size_t GetDataSize() const override { return m_size; }

    virtual size_t Read(uint8_t* output, size_t offset, size_t max) const override
    {
        if (!IsReadable()) return 0;
        size_t read = std::min(GetDataSize() - offset, max);
        m_stream.seekg(offset, std::ifstream::beg);
        m_stream.read(reinterpret_cast<char*>(output), read);
        return read;
    }
    
private:
    mutable std::ifstream m_stream;
    size_t m_size;
};

} // Df