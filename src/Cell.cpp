#include "Cell.h"

#include "FmeFile.h"

namespace Df
{

/*static*/ Cell Cell::CreateFromFile(const FmeFile& fme)
{
    bitmap_cache cache;
    return CreateFromFile(fme, cache);
}

/*static*/ Cell Cell::CreateFromFile(const FmeFile& fme, bitmap_cache& cache)
{
    std::shared_ptr<Bitmap> bitmap;
    auto key = fme.GetDataUid();
    auto found = cache.find(key);
    if (found == std::end(cache))
    {
        bitmap = std::make_shared<Bitmap>(fme.CreateBitmap());
        cache[key] = bitmap;
    }
    else
    {
        bitmap = found->second;
    }
    
     return Cell(
        fme.GetInsertX(),
        fme.GetInsertY(),
        fme.IsFlipped(),
        bitmap);
}

} // Df