#pragma once

#include <map>

namespace Df
{

enum class GoalType
{
    Unknown,
    Trigger,
    Item
};

struct Goal
{
    Goal() :
        type(GoalType::Unknown),
        value(-1)
    { }

    Goal(GoalType t, int v) :
        type(t),
        value(v)
    { }

    GoalType type;
    int value;
};

/**
    Collection of mission goals.
*/
class Gol
{
public:
    using GoalMap = std::map<size_t, Goal>;

    Gol() { }

    void AddGoal(size_t index, GoalType type, int value)
    {
        m_goals[index] = { type, value };
    }
    
    size_t NumberOfGoals() const { return m_goals.size(); }
    
    Goal GetGoal(size_t index) const
    {
        auto found = m_goals.find(index);
        if (found != std::end(m_goals))
        {
            return found->second;
        }
        else
        {
            return { GoalType::Unknown, -1 };
        }
    }
    
    auto begin() const
    {
        return std::begin(m_goals);
    }

    auto end() const
    {
        return std::end(m_goals);
    }

private:
    GoalMap m_goals;
};

} // Df