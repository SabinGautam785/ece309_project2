#include "conversation.h"
#include <cassert>
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
Conversation::Conversation(Conversation&& other) noexcept {
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}
Conversation& Conversation::operator=(Conversation&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = other.data_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
}
void Conversation::append(Message m) {
    if (size_ < capacity_) {
        data_[size_] = m;
        size_++;
    }
    else {
        std::size_t new_capacity;
        if (capacity_ == 0) {
            new_capacity = 1;
        }
        else {
            new_capacity = 2 * capacity_;
        }
        Message* new_data;
        new_data = new Message[new_capacity];
        for (std::size_t k = 0; k < size_; k++) {
            new_data[k] = data_[k];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
        data_[size_] = m;
        size_++;
    }
}
std::size_t Conversation::size() const noexcept {
    return size_;
}
const Message& Conversation::at(std::size_t i) const{
    assert(i < size_);
    return data_[i];
}