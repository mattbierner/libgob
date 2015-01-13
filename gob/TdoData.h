/**
    Types and data structures used in a 3DO file.
*/
#pragma once

#include <map>
#include <string>
#include <vector>

namespace Df
{

/**
    Type of shader used to render the 3DO.
*/
enum class TdoShadingType : unsigned
{
    Unknown,
    
    /** Normal, flat surface. */
    Flat,
    
    /** Gouraud shading on surface */
    Gouraud,
    
    /** Display only vertexes (like Death Star holo) */
    Vertex,
    
    /** Filled with a texture. */
    Texture,
    
    /** Filled with a texture plus Gouraud shading on texture. */
    Gourtex,
    
    /**
        Texture on a horizontal plane.
        
        Acts like a floor and ceiling textures. Texture must be 64x64 and is
        affected by flr and ceil txoffsets, and scrolled by elevators
        scroll_floor and scroll_ceiling.
    */
    Plane
};

/**
    Maps shading names used in 3DO definition to shading type.
*/
static const std::map<std::string, TdoShadingType> shadingNameMap = {
    {"FLAT", TdoShadingType::Flat},
    {"GOURAUD", TdoShadingType::Gouraud},
    {"VERTEX", TdoShadingType::Vertex},
    {"TEXTURE", TdoShadingType::Texture},
    {"GOURTEX", TdoShadingType::Gourtex},
    {"PLANE", TdoShadingType::Plane}
};

/**
    Index into the verticies table.
*/
using tdo_vertex_index = unsigned;

/**
    Index into the texture verticies table.
*/
using tdo_texture_vertex_index = unsigned;

/**
    Index into the texture table.
    
    -1 indicates no texture is used.
*/
using tdo_texture_index = int;

/**
    3DO color type.
    
    An index into a PAL file's color table.
*/
using tdo_color_index = uint8_t;

/**
*/
using tdo_coordinate = float;

/**
*/
using tdo_texture_coordinate = float;

/**
    Point in 3D space.
*/
struct TdoVertex
{
    tdo_coordinate x, y, z;
};

/**
    Collection of verticies.
*/
using TdoVerticies = std::vector<TdoVertex>;

/**
    2D texture position.
*/
struct TdoTextureVertex
{
    tdo_texture_coordinate x, y;
};

/**
    Collection of texture verticies.
*/
using TdoTextureVerticies = std::vector<TdoTextureVertex>;

/**
    Triangle.
*/
struct TdoTriangle
{
    tdo_vertex_index a, b, c;
    tdo_color_index color;
    TdoShadingType fill;
};

/**
    Collection of quads.
*/
using TdoTriangles = std::vector<TdoTriangle>;

/**
    Quad.
*/
struct TdoQuad
{
    tdo_vertex_index a, b, c, d;
    tdo_color_index color;
    TdoShadingType fill;
};

/**
    Collection of quads.
*/
using TdoQuads = std::vector<TdoQuad>;

/**
    Texturing quad.
*/
struct TdoTextureQuad
{
    tdo_texture_vertex_index a, b, c, d;
};

/**
    Collection of texture quads.
*/
using TdoTextureQuads = std::vector<TdoTextureQuad>;

/**
    Texturing triangle.
*/
struct TdoTextureTriangle
{
    tdo_texture_vertex_index a, b, c;
};

/**
    Collection of texture quads.
*/
using TdoTextureTriangles = std::vector<TdoTextureTriangle>;

/**
*/
using TdoTexture = std::string;

/**
*/
using TdoTextures = std::vector<std::string>;


/**
*/
struct TdoGeometry
{
    TdoVerticies verticies;
    TdoTriangles triangles;
    TdoQuads quads;
    TdoTextureVerticies textureVerticies;
    TdoTextureQuads textureQuads;
    TdoTextureTriangles textureTriangles;
};

/**
*/
struct TdoObject
{
    tdo_texture_index texture;
    TdoGeometry geometry;
};

} // Df