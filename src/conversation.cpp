#include "conversation.h"
Conversation::Conversation() : data_(nullptr), size_(0), capacity_(0) {
}
Conversation::~Conversation() {
    delete[] data_;
}
Conversation::Conversation(const Conversation& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new Message[other.capacity_];
    for (std::size_t i = 0; i < size_; i++) {
        data_[i] = other.data_[i];
    }
}
Conversation& Conversation::operator=(const Conversation& other) {
    if (this == &other) {
        return *this;
    }
    delete[] data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new Message[other.capacity_];
    for (std::size_t j = 0; j < size_; j++) {
        data_[j] = other.data_[j];
    }
    return *this;
}