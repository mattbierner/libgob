#include "GobFile.h"

namespace Df
{

/*static*/ const std::map<std::string, FileType> GobFile::fileTypeMap = {
// In-game graphics
    {"BM", FileType::Bm},
    {"FME", FileType::Fme},
    {"WAX", FileType::Wax},
    {"3DO", FileType::Tdo},
    {"VUE", FileType::Vue},
    {"PAL", FileType::Pal},
    {"CMP", FileType::Cmp},
    {"FNT", FileType::Fnt},

// In-game sound
    {"VOC", FileType::Voc},
    {"GMD", FileType::Gmd},

// In-game Level
    {"LEV", FileType::Lev},
    {"INF", FileType::Inf},
    {"Gol", FileType::Gol},
    {"O", FileType::O},

// Messages
    {"MSG", FileType::Msg}
};

/*static*/ GobFile GobFile::CreateFromFile(std::ifstream&& fs)
{
    return GobFile(std::make_unique<FileDataProvider>(std::move(fs)));
}

/*static*/ FileType GobFile::TypeForFileName(const std::string& filename)
{
    auto extPos = filename.find(".");
    if (extPos != std::string::npos)
    {
        std::string ext = filename.substr(extPos + 1);
        auto type = fileTypeMap.find(ext);
        if (type != std::end(fileTypeMap))
        {
            return type->second;
        }
    }
    return FileType::Unknown;
}

void GobFile::Init()
{
    auto index = GetIndex();
    for (unsigned i = 0; i < index.count; ++i)
    {
        auto entry = GetEntry(i);
        std::string filename(entry.filename);
        m_files.push_back(filename);
        m_entries[filename] = { TypeForFileName(filename), entry.offset, entry.size };
    }
}

GobFileIndex GobFile::GetIndex() const
{
    auto header = GetHeader();
    GobFileIndex index;
    Read<GobFileIndex>(&index, header.indexOffset);
    return index;
}

GobFileEntry GobFile::GetEntry(size_t i) const
{
    auto header = GetHeader();
    size_t startOffset = header.indexOffset + offsetof(GobFileIndex, entries);
    size_t offset = startOffset + (i * sizeof(GobFileEntry));

    GobFileEntry entry;
    Read<GobFileEntry>(&entry, offset);
    return entry;
}

void GobFile::Read(uint8_t* output, size_t offset, size_t max) const
{
    m_dataProvider->Read(output, offset, max);
}

} // Df