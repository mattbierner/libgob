#pragma once

#include <gob/Bitmap.h>
#include <gob/BmFileData.h>

#include <map>

namespace Df
{
    class FmeFile;
}

namespace Df
{

/**
    Mapping of bitmap Fme data UIDs to bitmap objects.
*/
using bitmap_cache = std::map<size_t, std::shared_ptr<Bitmap>>;

/**
    Single animation cell.
    
    Multiple cells may share a single bitmap (such as within a Wax).
*/
class Cell
{
public:
    /**
        Create a cell from a Fme file.
    */
    static Cell CreateFromFile(const FmeFile& fme);
    
    /**
        Create a cell from an Fme file.
        
        Attempts to get the bitmap from a cache or creates and inserts a
        new entry into the cache.
    */
    static Cell CreateFromFile(const FmeFile& fme, bitmap_cache& map);

    Cell(int32_t insertX, int32_t insertY, bool isFlipped, const std::shared_ptr<Bitmap>& bitmap) :
        m_insertX(insertX),
        m_insertY(insertY),
        m_isFlipped(isFlipped),
        m_bitmap(bitmap)
    { }
    
    int32_t GetInsertX() const { return m_insertX; }
    int32_t GetInsertY() const { return m_insertY; }
    
    /**
        Should the sprite be drawn flipped?
    */
    bool IsFlipped() const { return m_isFlipped; }

    /**
        Get the size of the a bitmap.
    */
    size_t GetDataSize() const
    {
        if (HasBitmap())
            return GetBitmap()->GetDataSize();
        else
            return 0;
    }
    
    /**
        Get the width of the sprite.
    */
    unsigned GetWidth() const
    {
        if (HasBitmap())
            return GetBitmap()->GetWidth();
        else
            return 0;
    }
    
    /**
        Get the height of the sprite.
    */
    unsigned GetHeight() const
    {
        if (HasBitmap())
            return GetBitmap()->GetHeight();
        else
            return 0;
    }
    
    /**
        Get the type of transparency used to render the sprite.
    */
    BmFileTransparency GetTransparency() const
    {
        if (HasBitmap())
            return GetBitmap()->GetTransparency();
        else
            return BmFileTransparency::Normal;
    }
    
    /**
        Does the cell have a valid bitmap?
    */
    bool HasBitmap() const
    {
        return (m_bitmap != nullptr);
    }
    
    /**
        Get the bitmap that stores the sprite's data.
    */
    std::shared_ptr<Bitmap> GetBitmap() const { return m_bitmap; }

private:
    std::shared_ptr<Bitmap> m_bitmap;
    int32_t m_insertX;
    int32_t m_insertY;
    bool m_isFlipped;
};

} // Df

