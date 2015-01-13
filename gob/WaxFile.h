#pragma once

#include <gob/Buffer.h>
#include <gob/DataReader.h>
#include <gob/FmeFile.h>
#include <gob/WaxFileData.h>

namespace Df
{

/**
    Animation sequence in a Wax file.
*/
class WaxFileSequence
{
public:
    WaxFileSequence(const std::shared_ptr<IBuffer>& data, size_t offset) :
        m_data(data),
        m_offset(offset)
    { }
    
    /**
        Get the number of frames in the sequence.
    */
    size_t GetFramesCount() const;
    
    /**
        Does a given frame exist?
    */
    bool HasFrame(size_t index) const
    {
        return (index < GetFramesCount());
    }
    
    /**
        Get the sprite FME associated with a given frame.
    */
    FmeFile GetFrame(size_t index) const;
    
    /**
        Get an id that uniquely identifies the image data within a Wax.
        
        Multiple frames may share the same image data but use different FME headers.
    */
    size_t GetDataUid(size_t index) const;

private:
    static WaxFileSequenceEntry s_emptyHeader;

    std::shared_ptr<IBuffer> m_data;
    size_t m_offset;
    
    /**
        Get the main file header.
    */
    const WaxFileSequenceEntry* GetHeader() const;
};

/**
    Single animated animation within a Wax file.
    
    Contains 32 views from different angles for the animation.
*/
class WaxFileAction
{
public:
    WaxFileAction(const std::shared_ptr<IBuffer>& data, size_t offset) :
        m_data(data),
        m_offset(offset)
    { }
    
    unsigned GetWorldWidth() const { return GetHeader()->worldWidth; }
    
    unsigned GetWorldHeight() const { return GetHeader()->worldHeight; }

    unsigned GetFrameRate() const { return GetHeader()->frameRate; }

    /**
        Get the number of views stored in the wax.
        
        Sequences are always densely packed.
    */
    size_t GetSequencesCount() const { return 32; }
    
    /**
        Get the animation for a specific view.
    */
    WaxFileSequence GetSequence(size_t index) const;
    
private:
    static WaxFileActionEntry s_emptyHeader;

    std::shared_ptr<IBuffer> m_data;
    size_t m_offset;
    
    /**
        Get the main file header.
    */
    const WaxFileActionEntry* GetHeader() const;
};

/**
    Wax file.
    
    Collection of animations for a sprite.
*/
class WaxFile
{
public:
    static WaxFile CreateFromDataProvider(const IDataReader& dataProvider)
    {
        return WaxFile(Buffer::CreateFromDataProvider(dataProvider));
    }
    
    WaxFile() { }
    
    WaxFile(Buffer&& data) :
        m_data(std::make_shared<Buffer>(std::move(data)))
    { }
    
    /**
        Get the indicies of all of the Wax's actions.
    */
    std::vector<size_t> GetActions() const;
    
    /**
        Does this Wax file have a given animation entry?
    */
    bool HasAction(size_t index) const;
    
    /**
        Get the animation entry for a given action.
    */
    WaxFileAction GetAction(size_t index) const;
    
private:
    static WaxFileHeader s_emptyHeader;

    std::shared_ptr<IBuffer> m_data;
    
    /**
        Get the main file header.
    */
    const WaxFileHeader* GetHeader() const;
};


} // Df

