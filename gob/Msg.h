#pragma once

#include <map>
#include <string>
#include <vector>

namespace Df
{

using message_index_t = size_t;

using message_priority_t = unsigned;

using message_contents_t = std::string;

/**
    Maps message identifiers to messages contents and metadata.
*/
class Msg
{
public:
    struct MessageEntry
    {
        message_priority_t priority;
        message_contents_t message;
    };
    
    Msg() { }
    
    /**
        Does a message for an index exists?
    */
    bool HasMessage(message_index_t index) const
    {
        auto found = m_messages.find(index);
        return (found != std::end(m_messages));
    }
    
    std::vector<message_index_t> GetKeys() const
    {
        std::vector<message_index_t> keys;
        std::transform(
            std::begin(m_messages),
            std::end(m_messages),
            std::back_inserter(keys),
            [](const auto& pair) { return pair.first; });
        return keys;
    }
    
    
    /**
     
    */
    size_t GetNumberMessages() const
    {
        return m_messages.size();
    }

    /**
        Get the message contents associated with an index.
        
        Returns an empty string if the messages does not exists.
    */
    message_contents_t GetMessage(message_index_t index) const
    {
        auto found = m_messages.find(index);
        if (found != std::end(m_messages))
            return found->second.message;
        else
            return { };
    }
    
    /**
        Get the priority of a message associated with an index.
        
        Return 0 if no such message exists.
    */
    message_priority_t GetPriority(message_index_t index) const
    {
        auto found = m_messages.find(index);
        if (found != std::end(m_messages))
            return found->second.priority;
        else
            return 0;
    }
    
    /**
        Insert a new message into the message map.
    */
    void AddMessage(message_index_t index, message_priority_t priority, const message_contents_t& message)
    {
        m_messages[index] = { priority, message };
    }
    
    void AddMessage(message_index_t index, message_priority_t priority, message_contents_t&& message)
    {
        m_messages[index] = { priority, std::move(message) };
    }
    
    auto begin() const { return std::begin(m_messages); }
    
    auto end() const { return std::end(m_messages); }
    
private:
    using message_map = std::map<message_index_t, MessageEntry>;

    message_map m_messages;
};

} // Df