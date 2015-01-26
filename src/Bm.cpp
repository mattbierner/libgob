#include "Bm.h"
#include "BmFile.h"

namespace Df
{

/*static*/ Bm Bm::CreateFromFile(const BmFile& bm)
{
    size_t count = bm.GetCountSubBms();
    
    std::vector<std::shared_ptr<Bitmap>> bitmaps(count);
    for (unsigned i = 0; i < count; ++i)
        bitmaps[i] = std::make_shared<Bitmap>(std::move(bm.CreateBitmap(i)));
    
    return Bm(
        bm.GetFrameRate(),
        bm.IsSwitch(),
        std::move(bitmaps));
}

} // Df