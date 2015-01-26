#include "GolFile.h"

#include <gob/Gol.h>
#include "ObjParser.h"

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/phoenix.hpp>

using namespace boost::spirit;
using namespace boost::spirit::qi;
namespace ph = boost::phoenix;

namespace Df
{

namespace GolParser
{

using Item = boost::tuple<bool, int>;

using Trigger = boost::tuple<bool, int>;

using Goal = boost::tuple<int, Item, Trigger>;

using Goals = std::vector<Goal>;


/**
    Gol file format parser.
*/
template <typename Iterator>
struct gol_parser : ObjParser<Iterator, Goals()>
{
    using base = ObjParser<Iterator, Goals()>;

    gol_parser() : base(start)
    {
        version = base::element("GOL", base::version_number);
        
        item = base::attribute("ITEM", int_[ph::at_c<0>(_val) = true]);
        
        trigger = base::attribute("TRIG", int_[ph::at_c<0>(_val) = true]);
        
        index = base::attribute("GOAL", int_);
        
        goal = base::token(boost::proto::deep_copy(index >> omit[+space] >> (item | trigger)));
        
        goals = *goal;
        
        start
            %= omit[version]
            >> goals;
    }
    
    rule<Iterator, Goals()> start;
    
    rule<Iterator> version;
    
    rule<Iterator, Goal()> goal;
    
    rule<Iterator, Goals()> goals;
    
    rule<Iterator, int()> index;
    rule<Iterator, Trigger()> trigger;
    rule<Iterator, Item()> item;
};

} // GolParser

Gol GolFile::CreateGol() const
{
    static const GolParser::gol_parser<const char*> p = { };
  
    const char* start = m_data.Begin<char>();
    const char* end = m_data.End<char>();

    GolParser::Goals goals;
    bool result = parse(start, end, p, goals);
    
    auto gol = Gol();
    if (result)
    {
        for (const auto& goal : goals)
        {
            const GolParser::Trigger& item = boost::get<1>(goal);
            const GolParser::Item& trigger = boost::get<2>(goal);

            GoalType type = (boost::get<0>(trigger) ? GoalType::Trigger : GoalType::Item);
            int value = (boost::get<0>(trigger) ? boost::get<1>(trigger) : boost::get<1>(item));
            gol.AddGoal(
                boost::get<0>(goal),
                type,
                value);
        }
    }
    return gol;
}

} // Df