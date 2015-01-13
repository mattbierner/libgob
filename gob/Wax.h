#pragma once

#include <gob/Cell.h>
#include <gob/WaxFile.h>

#include <array>
#include <map>
#include <vector>

namespace Df
{

/**
    Collection of frames of an animation.
*/
class WaxActionSequence
{
public:
    using action_frames = std::vector<Cell>;

    static WaxActionSequence CreateFromFile(const WaxFileSequence& seq);
    
    static WaxActionSequence CreateFromFile(const WaxFileSequence& seq, bitmap_cache& cache);
    
    WaxActionSequence(action_frames&& frames) :
        m_frames(std::move(frames))
    { }
    
    WaxActionSequence() { }
    
    /**
        Get the number of frames in the animation.
    */
    size_t GetFramesCount() const { return m_frames.size(); }
    
    /**
        Does a given frame exist in the animation?
    */
    bool HasFrame(size_t index) const
    {
        return (index < GetFramesCount());
    }
    
    /**
        Get the sprite FME associated with a given frame.
    */
    Cell GetFrame(size_t index) const { return m_frames[index]; }

private:
    action_frames m_frames;
};

/**
    Collection of views of an animation from different angles.
*/
class WaxAction
{
public:
    using action_views = std::array<WaxActionSequence, 32>;

    static WaxAction CreateFromFile(const WaxFileAction& action);
    
    static WaxAction CreateFromFile(const WaxFileAction& action, bitmap_cache& cache);

    WaxAction(action_views&& views, unsigned frameRate) :
        m_views(std::move(views)),
        m_frameRate(frameRate)
    { }
    
    unsigned GetFrameRate() const { return m_frameRate; }
    
    /**
        Get the number of views stored in the wax.
        
        Sequences are always densely packed.
    */
    size_t GetSequencesCount() const { return m_views.size(); }
    
    /**
        Get the animation for a specific view.
    */
    WaxActionSequence GetSequence(size_t index) const
    {
        return m_views[index];
    }
    
private:
    action_views m_views;
    unsigned m_frameRate;
};

/**
    Collection of animations for a sprite.
*/
class Wax
{
public:
    using action_map = std::map<size_t, WaxAction>;

    static Wax CreateFromFile(const WaxFile& wax);
    
    Wax(action_map&& actions) :
        m_actions(std::move(actions))
    { }
    
    /**
        Get list of actions.
    */
    std::vector<size_t> GetActions() const;
    
    /**
        Does a given action exist?
    */
    bool HasAction(size_t index) const
    {
        return (m_actions.find(index) != std::end(m_actions));
    }
    
    /**
        Get the action associated with a given id.
    */
    WaxAction GetAction(size_t index) const { return m_actions.at(index); }
    
private:
    action_map m_actions;
};

} // Df