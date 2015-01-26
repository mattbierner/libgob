#include "MsgFile.h"

#include <gob/Msg.h>
#include "ObjParser.h"

#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

using namespace boost::spirit::qi;

namespace Df
{

/**
*/
using message = boost::tuple<message_index_t, message_priority_t, message_contents_t>;

/**
*/
using message_list = std::vector<message>;


/**
    MSG file format parser.
*/
template <typename Iterator>
struct msg_parser : ObjParser<Iterator, message_list()>
{
    using base = ObjParser<Iterator, message_list()>;

    msg_parser() : base(start)
    {
        version = base::element("MSG", base::version_number);
        
        count = base::element("MSGS", int_);
        
        message %= base::element(int_, base::attribute(int_, +base::quoted_string));
        
        contents %= *base::token(boost::proto::deep_copy(message));
        
        end %= lit("END");
        
        start
            %= omit[version]
            >> omit[*base::comment_or_space]
            >> omit[count]
            >> contents
            >> omit[end];
    }
    
    rule<Iterator, message_list()> start;
    rule<Iterator, float()> version;
    rule<Iterator, size_t()> count;
    rule<Iterator, message_list()> contents;
    rule<Iterator, message()> message;
    rule<Iterator> end;
};
    
Msg MsgFile::CreateMsg() const
{
    static const msg_parser<const char*> p = { };

    const char* start = m_data.Begin<char>();
    const char* end = m_data.End<char>();
    
    message_list messages;
    bool result = parse(start, end, p, messages);
    
    Msg messageObject = { };
    if (result)
    {
        for (const auto& message : messages)
        {
            messageObject.AddMessage(
                boost::get<0>(message),
                boost::get<1>(message),
                std::move(boost::get<2>(message)));
        }
    }
    
    return messageObject;
}

} // Df