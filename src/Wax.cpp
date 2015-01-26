#include "Wax.h"

#include <boost/range/counting_range.hpp>

namespace Df
{

/*static*/ WaxActionSequence WaxActionSequence::CreateFromFile(const WaxFileSequence& seq)
{
    bitmap_cache cache;
    return CreateFromFile(seq, cache);
}

/*static*/ WaxActionSequence WaxActionSequence::CreateFromFile(const WaxFileSequence& seq, bitmap_cache& cache)
{
    action_frames frames;
    for (size_t i : boost::counting_range(0ul, seq.GetFramesCount()))
        frames.emplace_back(Cell::CreateFromFile(seq.GetFrame(i), cache));
    
    return WaxActionSequence(std::move(frames));
}

/*static*/ WaxAction WaxAction::CreateFromFile(const WaxFileAction& action)
{
    bitmap_cache cache;
    return CreateFromFile(action, cache);
}

/*static*/ WaxAction WaxAction::CreateFromFile(const WaxFileAction& action, bitmap_cache& cache)
{
    action_views views;
    for (size_t i : boost::counting_range(0ul, action.GetSequencesCount()))
        views[i] = WaxActionSequence::CreateFromFile(action.GetSequence(i), cache);
    
    return WaxAction(
        std::move(views),
        action.GetFrameRate());
}

/*static*/ Wax Wax::CreateFromFile(const WaxFile& wax)
{
    // Waxes contain multiple ptrs to the same bitmaps internally, so build a map
    // to avoid creating redundant bitmap objects.
    bitmap_cache cache;
    
    action_map actions;
    for (size_t i : wax.GetActions())
    {
        auto action = wax.GetAction(i);
        actions.emplace(i, WaxAction::CreateFromFile(action, cache));
    }
    return Wax(std::move(actions));
}

std::vector<size_t> Wax::GetActions() const
{
    std::vector<size_t> indicies;
    std::transform(
        std::begin(m_actions),
        std::end(m_actions),
        std::back_inserter(indicies),
        [](const auto& pair) { return pair.first; });
    return indicies;
}
    
}