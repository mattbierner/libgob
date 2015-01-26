#pragma once

#include <map>
#include <vector>

#include <gob/GobFileData.h>
#include <gob/DataReader.h>

namespace Df
{

/**
    Type of a file stored in a Gob.
*/
enum class FileType
{
    Unknown,
    
// In-game graphics
    Bm,
    Fme,
    Wax,
    Tdo,
    Vue,
    Pal,
    Cmp,
    Fnt,

// In-game sound
    Voc,
    Gmd,
    
// In-game level
    Lev,
    Inf,
    Gol,
    O,
    
// Messages
    Msg
};

/**
    Gob file.
    
    Allows reading from a Gob file.
*/
class GobFile
{
    struct Entry
    {
        FileType type;
        uint32_t offset;
        uint32_t size;
    };
    
    using FileMap = std::map<std::string, Entry>;

    using FileExtMap = std::map<std::string, FileType>;

public:
    /**
        Create a Gob file from data in a file stream.
    */
    static GobFile CreateFromFile(std::ifstream&& fs);

    /**
        For a given filename, guess its file type based on its extension.
    */
    static FileType GetFileType(const std::string& filename);

    GobFile() { }

    GobFile(std::unique_ptr<IDataReader>&& dataProvider) :
        m_dataProvider(std::move(dataProvider))
    {
        Init();
    }

    /**
        Get a list of all filenames in the Gob.
    */
    std::vector<std::string> GetFilenames() const
    {
        return m_files;
    }
    
    /**
        Get a list of all filenames in the Gob.
    */
    std::vector<std::string> GetFilenamesOfType(FileType type) const
    {
        std::vector<std::string> list;
        for (const auto& mapping : m_entries)
            if (mapping.second.type == type)
                list.push_back(mapping.first);
        return m_files;
    }
    
    /**
        Get the name of the file at `index`?
    */
    std::string GetFilename(size_t index) const
    {
        if (index < m_files.size())
            return m_files[index];
        else
            return std::string();
    }
    
    /**
       Does an entry for `filename` exist?
    */
    bool HasFile(const std::string& filename) const
    {
        return (m_entries.find(filename) != std::end(m_entries));
    }
    
    /**
        Get the size of a file in the Gob.
        
        Returns 0 if the file does not exist.
    */
    size_t GetFileSize(const std::string& filename) const
    {
        if (HasFile(filename))
            return GetFile(filename).size;
        else
            return 0;
    }
    
    /**
        Read part of a file into a buffer.
     
        @param filename Name of file to read.
        @param buffer Memory to write result to.
        @param offset Relative offset within the file.
        @param max Maximum number of bytes to read.
    */
    size_t ReadFile(const std::string& filename, uint8_t* buffer, size_t offset, size_t max) const
    {
        if (!HasFile(filename))
            return 0;
        
        auto entry = GetFile(filename);
        if (offset > entry.size)
            return 0;
        
        size_t read = std::min(entry.size - offset, max);
        Read(buffer, entry.offset + offset, read);
        return read;
    }
    
private:
    /**
        Maps file extensions used in dark forces to file types.
    */
    static const FileExtMap fileTypeMap;

    std::unique_ptr<IDataReader> m_dataProvider;
    std::vector<std::string> m_files;
    FileMap m_entries;
  
    /**
        Initilize the internal data structures using the data provider.
    */
    void Init();
    
    /**
        Get file entry information.
    */
    Entry GetFile(const std::string& filename) const
    {
        return m_entries.at(filename);
    }
    
    /**
        Read the file header.
    */
    GobFileHeader GetHeader() const
    {
        GobFileHeader header;
        Read<GobFileHeader>(&header, 0);
        return header;
    }
    
    /**
        Read the file index.
    */
    GobFileIndex GetIndex() const;
    
    /**
        Read an entry in the file index.
    */
    GobFileEntry GetEntry(size_t i) const;
    
    /**
        Read an object of type `T` from the Gob.
    */
    template <typename T>
    void Read(T* output, size_t offset) const
    {
        Read(reinterpret_cast<uint8_t*>(output), offset, sizeof(T));
    }
    
    /**
        Read `max` bytes at absolute position `offset` from the gob.
    */
    void Read(uint8_t* output, size_t offset, size_t max) const;
};

} // Df

