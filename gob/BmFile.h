#pragma once

#include <gob/Bitmap.h>
#include <gob/BmFileData.h>
#include <gob/DataReader.h>
#include <gob/Buffer.h>

namespace Df
{

/**
    Bitmap file.
    
    May potentially contain multiple sub bitmaps.
*/
class BmFile
{
public:
    /**
        Create a BM file from data in a file stream.
    */
    static BmFile CreateFromDataProvider(const IDataReader& dataProvider)
    {
        return BmFile(Buffer::CreateFromDataProvider(dataProvider));
    }
    
    BmFile() { }
    
    BmFile(Buffer&& data) :
        m_data(std::make_shared<Buffer>(std::move(data)))
    { }
    
    /**
        Size of the uncompressed BM.
    */
    size_t GetDataSize(size_t index) const
    {
        return GetWidth(index) * GetHeight(index);
    }
    
    /**
        Get the width of the image.
        
        @param index Sub BM to get width of. Only valid for multiple BMs.
    */
    unsigned GetWidth(size_t index = 0) const
    {
        return (IsMultipleBm() ? GetSubHeader(index)->sizeX : GetHeader()->sizeX);
    }
    
    /**
        Get the height of the image.
        
        @param index Sub BM to get height of. Only valid for multiple BMs.
    */
    unsigned GetHeight(size_t index = 0) const
    {
        return (IsMultipleBm() ? GetSubHeader(index)->sizeY : GetHeader()->sizeY);
    }
    
    /**
        Get the type of transparency of the image.
     
        @param index Sub BM to get ransparency of. Only valid for multiple BMs.
    */
    BmFileTransparency GetTransparency(size_t index = 0) const
    {
        return (IsMultipleBm() ? GetSubHeader(index)->transparency : GetHeader()->transparency);
    }
    
    /**
        Does this file contain sub BMs?
    */
    bool IsMultipleBm() const
    {
        const auto* header = GetHeader();
        return (header->sizeX == 1 && header->sizeY != 1);
    }
    
    /**
        Get the number of sub BM stored in this file.
        
        Returns 1 for non-multiple BMs.
    */
    unsigned GetCountSubBms() const
    {
        return (IsMultipleBm() ? GetHeader()->idemY : 1);
    }
    
    /**
        Get the frame rate setting of a multiple BM.
        
        Returns 0 for non-multiple BMs and for switches.
    */
    uint8_t GetFrameRate() const;
    
    /**
        Is a multiple BM a switch?
        
        Returns false for non-multiple BMs.
    */
    bool IsSwitch() const
    {
        return (IsMultipleBm() && GetFrameRate() == 0);
    }

    /**
        Uncompress the bitmap file.
        
        @param index Index of subbitmap to uncompress
    */
    Bitmap CreateBitmap(size_t index = 0) const;
    
private:
    static BmFileHeader s_emptyHeader;
    static BmFileSubHeader s_emptySubHeader;
    
    std::shared_ptr<IBuffer> m_data;
    
    /**
        Get the main file header.
    */
    const BmFileHeader* GetHeader() const;
    
    /**
        Get the sub header for a multiple BM.
     
        Only valid for multiple BM.
    */
    const BmFileSubHeader* GetSubHeader(size_t index) const;
    
    /**
        Read an uncompress the bitmap data.
    */
    Buffer Uncompress(size_t index) const;
    
    /**
        Get the type of compression used on the bitmap data.
    */
    BmFileCompression GetCompression() const
    {
        return (IsMultipleBm() ? BmFileCompression::None : GetHeader()->compression);
    }
    
    /**
        Is the bitmap data compressed?
    */
    bool IsCompressed() const
    {
        return (GetCompression() != BmFileCompression::None);
    }
    
    /**
        For a multiple BM, get the absolute offset to the start of the sub file.
    */
    int32_t GetSubOffset(size_t index) const;
    
    /**
        Get a pointer to the start of image data.
    */
    const size_t GetImageDataStart(size_t index) const;
};

} // Df

