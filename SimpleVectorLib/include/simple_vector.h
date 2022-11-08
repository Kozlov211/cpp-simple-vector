#pragma once

#include "array_ptr.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <utility>

namespace simple_vector {

class ReserveProxyObj{
public:
    ReserveProxyObj(size_t capacity);

    size_t Capacity() const noexcept;
private:
    size_t capacity_;
};

inline ReserveProxyObj::ReserveProxyObj(size_t capacity) : capacity_(capacity) {};

inline size_t ReserveProxyObj::Capacity() const noexcept{
    return capacity_;
}

inline ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size);

    SimpleVector(size_t size, const Type& value);

    SimpleVector(std::initializer_list<Type> initializer_list);

    SimpleVector(ReserveProxyObj obj_capacity);

    SimpleVector(const SimpleVector& other);

    SimpleVector& operator=(const SimpleVector& rhs);

    SimpleVector(SimpleVector&& other) noexcept;

    SimpleVector& operator=(SimpleVector&& other) noexcept;

    void PushBack(const Type& item);

    void PushBack(Type&& item);

    Iterator Insert(ConstIterator pos, const Type& value);

    Iterator Insert(ConstIterator pos, Type&& value);

    Iterator Erase(ConstIterator pos);

    void Resize(size_t new_size);

    void PopBack() noexcept;

    void swap(SimpleVector& other) noexcept;

    void Reserve(size_t new_capacity);

    size_t GetSize() const noexcept;

    size_t GetCapacity() const noexcept;

    bool IsEmpty() const noexcept;

    Type& operator[](size_t index) noexcept;

    const Type& operator[](size_t index) const noexcept;

    Type& At(size_t index);

    const Type& At(size_t index) const;

    Type& Front() noexcept;

    const Type& Front() const noexcept;

    Type& Back() noexcept;

    const Type& Back() const noexcept;

    Type* Data() const noexcept;

    void Clear() noexcept;

    Iterator begin() noexcept;

    Iterator end() noexcept;

    ConstIterator begin() const noexcept;

    ConstIterator end() const noexcept;

    ConstIterator cbegin() const noexcept;

    ConstIterator cend() const noexcept;

private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    array_ptr::ArrayPtr<Type> array_ptr_;
};

template <typename Type>
SimpleVector<Type>::SimpleVector(size_t size) : size_(size), capacity_(size), array_ptr_(new Type[size_]{}) {}

template <typename Type>
SimpleVector<Type>::SimpleVector(size_t size, const Type& value) : size_(size), capacity_(size), array_ptr_(size_) {
    std::fill(begin(), end(), value);
}

template <typename Type>
SimpleVector<Type>::SimpleVector(std::initializer_list<Type> initializer_list) : size_(initializer_list.size()), capacity_(initializer_list.size())  {
    array_ptr::ArrayPtr<Type> new_array_ptr(size_);
    std::copy(initializer_list.begin(), initializer_list.end(), new_array_ptr.Get());
    array_ptr_.swap(new_array_ptr);
}

template <typename Type>
SimpleVector<Type>::SimpleVector(ReserveProxyObj obj_capacity) {
    capacity_ = obj_capacity.Capacity();
}

template <typename Type>
SimpleVector<Type>::SimpleVector(const SimpleVector& other) {
    array_ptr::ArrayPtr<Type> new_array_ptr(other.capacity_);
    std::copy(other.begin(), other.end(), new_array_ptr.Get());
    array_ptr_.swap(new_array_ptr);
    size_ = other.size_;
    capacity_ = other.capacity_;
}

template <typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(const SimpleVector& rhs)  {
    if (this != &rhs) {
        array_ptr::ArrayPtr<Type> new_array_ptr(rhs.size_);
        std::copy(rhs.begin(), rhs.end(), new_array_ptr.Get());
        array_ptr_.swap(new_array_ptr);
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
    }
    return *this;
}

template <typename Type>
SimpleVector<Type>::SimpleVector(SimpleVector&& other) noexcept {
    array_ptr_.swap(other.array_ptr_);
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
}

template <typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(SimpleVector&& other) noexcept {
    if (this != &other) {
        array_ptr_.swap(other.array_ptr_);
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    }
    return *this;
}

template <typename Type>
void SimpleVector<Type>::PushBack(const Type& item) {
    Insert(end(), item);
}


template <typename Type>
void SimpleVector<Type>::PushBack(Type&& item) {
    Insert(end(), std::move(item));
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, const Type& value) {
    assert(pos >= begin() && pos <= end());
    if (size_ == capacity_) {
        array_ptr::ArrayPtr<Type> new_array_ptr(size_ == 0 ? 1 : size_ * 2);
        size_t index_pos = pos - cbegin();
        Iterator old_pos = begin() + index_pos;
        Iterator new_pos = std::copy(begin(), old_pos, new_array_ptr.Get());
        *new_pos = value;
        std::copy_backward(old_pos, end(), new_array_ptr.Get() + size_ + 1);
        capacity_ = size_ == 0 ? 1 : size_ * 2;
        ++size_;
        array_ptr_.swap(new_array_ptr);
        return new_pos;
    } else {
        size_t index_pos = pos - cbegin();
        Iterator new_pos = begin() + index_pos;
        std::copy(new_pos, end(), new_pos + 1);
        *new_pos = value;
        ++size_;
        return new_pos;
    }
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, Type&& value) {
    assert(pos >= begin() && pos <= end());
    if (size_ == capacity_) {
        array_ptr::ArrayPtr<Type> new_array_ptr(size_ == 0 ? 1 : size_ * 2);
        size_t index_pos = pos - cbegin();
        Iterator old_pos = begin() + index_pos;
        Iterator new_pos = std::move(begin(), old_pos, new_array_ptr.Get());
        *new_pos = std::move(value);
        std::move_backward(old_pos, end(), new_array_ptr.Get() + size_ + 1);
        capacity_ = size_ == 0 ? 1 : size_ * 2;
        ++size_;
        array_ptr_.swap(new_array_ptr);
        return new_pos;
    } else {
        size_t index_pos = pos - cbegin();
        Iterator new_pos = begin() + index_pos;
        std::move(new_pos, end(), new_pos + 1);
        *new_pos = std::move(value);
        ++size_;
        return new_pos;
    }
}

template <typename Type>
void SimpleVector<Type>::Resize(size_t new_size) {
    if (new_size > capacity_) {
            capacity_ = new_size;
            array_ptr::ArrayPtr<Type> new_array_ptr(capacity_);
            std::move(begin(), end(), new_array_ptr.Get());
            array_ptr_.swap(new_array_ptr);
            size_ = std::move(new_size);
    } else {
        if (new_size > size_) {
            Iterator it = array_ptr_.Get() + size_;
            while (it != array_ptr_.Get() + new_size) {
                *it = std::move(Type());
                ++it;
            }
        }
        size_ = new_size;
    }
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Erase(ConstIterator pos) {
    assert(pos >= begin() && pos <= end());
    size_t index_pos = pos - cbegin();
    Iterator old_pos = begin() + index_pos;
    std::move(old_pos + 1, end(), old_pos);
    --size_;
    return old_pos;
}

template <typename Type>
void SimpleVector<Type>::PopBack() noexcept {
    if (!IsEmpty()) {
        --size_ ;
    }
}

template <typename Type>
void SimpleVector<Type>::swap(SimpleVector& other) noexcept {
    array_ptr_.swap(other.array_ptr_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template <typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity) {
if (new_capacity > capacity_) {
        array_ptr::ArrayPtr<Type> new_array_ptr(new_capacity);
        std::copy(begin(), end(), new_array_ptr.Get());
        array_ptr_.swap(new_array_ptr);
        capacity_ = new_capacity;
    }
}

template <typename Type>
size_t SimpleVector<Type>::GetSize() const noexcept {
    return size_;
}

template <typename Type>
size_t SimpleVector<Type>::GetCapacity() const noexcept {
    return capacity_;
}

template <typename Type>
bool SimpleVector<Type>::IsEmpty() const noexcept {
    return (size_ == 0 ? true : false);
}

template <typename Type>
Type& SimpleVector<Type>::operator[](size_t index) noexcept {
    assert(index < size_);
    return array_ptr_[index];
}

template <typename Type>
const Type& SimpleVector<Type>::operator[](size_t index) const noexcept {
    assert(index < size_);
    return array_ptr_[index];
}

template <typename Type>
Type& SimpleVector<Type>::At(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("out_of_range");
    }
    return array_ptr_[index];
}

template <typename Type>
const Type& SimpleVector<Type>::At(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("out_of_range");
    }
    return array_ptr_[index];
}

template <typename Type>
Type& SimpleVector<Type>::Front() noexcept {
    return array_ptr_[0];
}

template <typename Type>
const Type& SimpleVector<Type>::Front() const noexcept {
    return array_ptr_[0];
}

template <typename Type>
Type& SimpleVector<Type>::Back() noexcept {
    return array_ptr_[size_ - 1];
}

template <typename Type>
const Type& SimpleVector<Type>::Back() const noexcept {
    return array_ptr_[size_ - 1];
}

template <typename Type>
Type* SimpleVector<Type>::Data() const noexcept {
    return array_ptr_.Get();
}

template <typename Type>
void SimpleVector<Type>::Clear() noexcept {
    size_ = 0;
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::begin() noexcept {
    return array_ptr_.Get();
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::end() noexcept {
    return (IsEmpty() ? begin() : array_ptr_.Get() + size_);
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::begin() const noexcept {
    return array_ptr_.Get();
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::end() const noexcept {
    return (IsEmpty() ? begin() : array_ptr_.Get() + size_);
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cbegin() const noexcept {
    return array_ptr_.Get();
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cend() const noexcept {
    return (IsEmpty() ? begin() : array_ptr_.Get() + size_);
}

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (lhs.GetSize() != rhs.GetSize() ? false : std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lhs < rhs || lhs == rhs;
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs <= rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}

} // namespace simple_vector

