/* Copyright 2016 Zachary Krausman */
#ifndef RING_BUFFER_CLASS_CPP_INCLUDED
#define RING_BUFFER_CLASS_CPP_INCLUDED
#include "ring_buffer_class.hpp"

namespace ringbuffer {
template <typename T, int N>
ringBuffer<T, N>::ringBuffer() : capacity_(N), begin_(0), size_(0) {
    buffer_ = new T[N];
}

template <typename T, int N>
void ringBuffer<T, N>::push_back(const T &element) {
if(size_ == capacity_) {
  throw std::runtime_error("can't add to a full ring");
}
        buffer_[(begin_+size_)% capacity_] = element;
        size_ = size_ + 1;
}
template <typename T, int N>
T ringBuffer<T, N>::pop_front() {
if(size_ == 0 || capacity_ == 0) {
  throw std::runtime_error("can't remove from an empty ring");
}
        T temp = buffer_[begin_];
        size_ = size_ - 1;
        if (begin_ == capacity_) {
            begin_ = 0;
        } else {
            begin_ = begin_ + 1;
        }
        return temp;
}

template <typename T, int N>
ringBuffer<T, N>::~ringBuffer() {
    delete[] buffer_;
}

template <typename T, int N>
bool ringBuffer<T, N>::isEmpty() {
    if (size_ == 0 || capacity_ == 0) {
        return 1;
    }
    return 0;
}

template <typename T, int N>
bool ringBuffer<T, N>::isFull() {
    if (size_ == capacity_) {
        return 1;
    }
    return 0;
}
}  // namespace ringbuffer
#endif  // RING_BUFFER_CLASS_CPP_INCLUDED
