#pragma once

#include <map>

namespace Df
{

enum class GoalType
{
    Trigger,
    Item
};

struct Goal
{
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