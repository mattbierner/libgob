#pragma once

#include <gob/Bitmap.h>
#include <gob/BmFileData.h>

namespace Df
{
    class BmFile;
}


namespace Df
{

/**
    Bitmap texture.
    
    May contain multiple sub bitmaps.
*/
class Bm
{
public:
    /**
        Create a bitmap from a file.
    */
    static Bm CreateFromFile(const BmFile& bm);
    
    Bm(unsigned frameRate, bool isSwitch, std::vector<std::shared_ptr<Bitmap>>&& bitmaps) :
        m_frameRate(frameRate),
        m_isSwitch(isSwitch),
        m_bitmaps(std::move(bitmaps))
    { }
    
    /**
        Get the frame rate setting of a multiple BM.
    */
    uint8_t GetFrameRate() const { return m_frameRate; }
    
    /**
        Is a multiple BM a switch?
    */
    bool IsSwitch() const { return m_isSwitch; }

    /**
        Does this file contain sub bitmaps?
    */
    bool IsMultipleBm() const
    {
        return (GetCountSubBms() > 1);
    }
    
    /**
        Get the number of sub bitmaps.
    */
    size_t GetCountSubBms() const
    {
        return m_bitmaps.size();
    }

    /**
        Size of the a bitmap.
        
        @param index Sub bitmap to get size of.
    */
    size_t GetDataSize(size_t index = 0) const
    {
        if (HasBitmap(index))
            return GetBitmap(index)->GetDataSize();
        else
            return 0;
    }
    
    /**
        Get the width of the image.
        
        @param index Sub bitmap to get width of.
    */
    unsigned GetWidth(size_t index = 0) const
    {
        if (HasBitmap(index))
            return GetBitmap(index)->GetWidth();
        else
            return 0;
    }
    
    /**
        Get the height of the image.
        
        @param index Sub BM to get height of.
    */
    unsigned GetHeight(size_t index = 0) const
    {
        if (HasBitmap(index))
            return GetBitmap(index)->GetHeight();
        else
            return 0;
    }
    
    /**
        Get the type of transparency of the image.
     
        @param index Sub bitmap to get transparency of.
    */
    BmFileTransparency GetTransparency(size_t index = 0) const
    {
        if (HasBitmap(index))
            return GetBitmap(index)->GetTransparency();
        else
            return BmFileTransparency::Normal;
    }
    
    /**
        Does a given sub bitmap exist?
    */
    bool HasBitmap(size_t index = 0) const
    {
        return (index < GetCountSubBms() && m_bitmaps[index]);
    }
    
    /**
        Get a given subbitmap.
    */
    std::shared_ptr<Bitmap> GetBitmap(size_t index = 0) const
    {
        if (HasBitmap(index))
            return m_bitmaps.at(index);
        else
            return nullptr;
    }

private:
    std::vector<std::shared_ptr<Bitmap>> m_bitmaps;
    unsigned m_frameRate;
    bool m_isSwitch;
};

} // Df

