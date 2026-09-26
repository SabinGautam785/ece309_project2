#ifndef CONVERSATION_H
#define CONVERSATION_H
#include "message.h"
#include <cstddef>
class Conversation {
    public:
    Conversation();
    ~Conversation();
    Conversation(const Conversation& other);
    Conversation& operator = (const Conversation& other);
    Conversation(Conversation&& other) noexcept;
    Conversation& operator = (Conversation&& other) noexcept;
    void append(Message m);
    std::size_t size() const noexcept;
    const Message& at(std::size_t index) const;
    const Message* begin() const noexcept;
    const Message* end() const noexcept;
    private:
    Message* data_;
    std::size_t size_;
    std::size_t capacity_;
};


#endif