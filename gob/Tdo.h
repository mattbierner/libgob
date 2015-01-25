#pragma once

#include <gob/TdoData.h>

namespace Df
{

/**
    3DO object.
*/
class Tdo
{
public:
    Tdo() { }
    
    Tdo(TdoTextures&& textures, std::vector<TdoObject>&& objects) :
        m_textures(std::move(textures)),
        m_objects(std::move(objects))
    { }

// Objects
    /**
        Get the number of objects in the 3DO.
    */
    size_t GetObjectsCount() const { return m_objects.size(); }
    
    /**
        Access an object in the 3DO.
    */
    const TdoObject& GetObject(size_t index) const { return m_objects[index]; }
    
// Textures
    /**
        Get the number of textures stored.
    */
    size_t GetTexturesCount() const { return m_textures.size(); }
    
    /**
        Does a given texture exist?
    */
    bool HasTexture(size_t index) const { return (index < GetTexturesCount()); }
    
    /**
        Lookup the name of the texture associated with given index.
    */
    TdoTexture GetTexture(size_t index) const { return m_textures[index]; }
    
    /**
        Get the list of textures
    */
    const TdoTextures& GetTextures() const { return m_textures; }

private:
    TdoTextures m_textures;
    std::vector<TdoObject> m_objects;
};

} // Df