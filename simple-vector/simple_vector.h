#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "array_ptr.h"

namespace simple_vector {

class ReserveProxyObj{
public:
    ReserveProxyObj(size_t capacity) : capacity_(capacity) {};

    size_t Capacity() {
        return capacity_;
    }
private:
    size_t capacity_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size) : size_(size), capacity_(size) {
        array_ptr::ArrayPtr<Type> tmp(new Type[size_]{});
        vector_.swap(tmp);
    }

    SimpleVector(const SimpleVector& other) {
        array_ptr::ArrayPtr<Type> tmp(new Type[other.capacity_]{});
        std::swap(other.begin(), other.end(), tmp.Get());
        vector_.swap(tmp);
        size_ = other.size_;
        capacity_ = other.capacity_;
    }

    SimpleVector& operator=(const SimpleVector& rhs)  {
        if (this != &rhs) {
            array_ptr::ArrayPtr<Type> tmp(new Type[rhs.capacity_]{});
            std::swap(rhs.begin(), rhs.end(), tmp.Get());
            vector_.swap(tmp);
            size_ = rhs.size_;
            capacity_ = rhs.capacity_;
        }
        return *this;
    }

    SimpleVector(SimpleVector&& other) noexcept {
        vector_.swap(other.vector_);
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    };

    SimpleVector& operator=(SimpleVector&& other) noexcept {
        if (this != &other) {
            vector_.swap(other.vector_);
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
        }
        return *this;
    }

    void PushBack(const Type& item) {
        Insert(end(), item);
    }

    void PushBack(Type&& item) {
        Insert(end(), std::move(item));
    }

    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= begin() && pos <= end());
        if (capacity_ == 0) {
            array_ptr::ArrayPtr<Type> tmp(new Type[1]{});
            tmp[0] = value;
            vector_.swap(tmp);
            capacity_ = 1;
            size_ = 1;
            return begin();
        } else if (size_ == capacity_) {
            array_ptr::ArrayPtr<Type> tmp(new Type[capacity_ * 2]{});
            size_t index_pos = pos - cbegin();
            Iterator old_pos = begin() + index_pos;
            Iterator new_pos = std::copy(begin(), old_pos, tmp.Get());
            *new_pos = value;
            std::copy_backward(old_pos, end(), tmp.Get() + size_ + 1);
            ++size_;
            capacity_ *= 2;
            vector_.swap(tmp);
            return new_pos;
        } else {
            size_t index_pos = pos - cbegin();
            Iterator old_pos = begin() + index_pos;
            std::copy(old_pos, end(), old_pos + 1);
            *old_pos = value;
            ++size_;
            return old_pos;
        }
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= begin() && pos <= end());
        if (capacity_ == 0) {
            array_ptr::ArrayPtr<Type> tmp(new Type[1]{});
            tmp[0] = std::move(value);
            vector_.swap(tmp);
            capacity_ = 1;
            size_ = 1;
            return begin();
        } else if (size_ == capacity_) {
            array_ptr::ArrayPtr<Type> tmp(new Type[capacity_ * 2]{});
            size_t index_pos = pos - cbegin();
            Iterator old_pos = begin() + index_pos;
            Iterator new_pos = std::move(begin(), old_pos, tmp.Get());
            *new_pos = std::move(value);
            std::move_backward(old_pos, end(), tmp.Get() + size_ + 1);
            ++size_;
            capacity_ *= 2;
            vector_.swap(tmp);
            return new_pos;
        } else {
            size_t index_pos = pos - cbegin();
            Iterator old_pos = begin() + index_pos;
            std::move(old_pos, end(), old_pos + 1);
            *old_pos = std::move(value);
            ++size_;
            return old_pos;
            }
    }

    void PopBack() noexcept {
        if (!IsEmpty()) {
            --size_ ;
        }
    }

    Iterator Erase(ConstIterator pos) {
        assert(pos >= begin() && pos <= end());
        size_t index_pos = pos - cbegin();
        Iterator old_pos = begin() + index_pos;
        std::move(old_pos + 1, end(), old_pos);
        --size_;
        return old_pos;
    }

    void swap(SimpleVector& other) noexcept {
        vector_.swap(other.vector_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    SimpleVector(size_t size, const Type& value) : size_(size), capacity_(size) {
        array_ptr::ArrayPtr<Type> tmp(new Type[size_]{});
        vector_.swap(tmp);
        std::fill(begin(), end(), value);
    }

    SimpleVector(std::initializer_list<Type> init) : size_(init.size()), capacity_(init.size())  {
        array_ptr::ArrayPtr<Type> tmp(new Type[size_]{});
        vector_.swap(tmp);
        std::copy(init.begin(), init.end(), tmp.Get());
    }

    SimpleVector(ReserveProxyObj obj_capacity) {
        capacity_ = obj_capacity.Capacity();
    }

    size_t GetSize() const noexcept {
        return size_;
    }

    void Reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
            array_ptr::ArrayPtr<Type> tmp(new Type[new_capacity]{});
            std::copy(begin(), end(), tmp.Get());
            vector_.swap(tmp);
            capacity_ = new_capacity;
        }
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return (size_ == 0 ? true : false);
    }

    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return vector_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return vector_[index];
    }

    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("out_of_range");
        }
        return vector_[index];
    }

    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("out_of_range");
        }
        return vector_[index];
    }

    void Clear() noexcept {
        size_ = 0;
    }

    void Resize(size_t new_size) {
        if (new_size > capacity_) {
                capacity_ = new_size;
                array_ptr::ArrayPtr<Type> tmp(new Type[capacity_]{});
                std::move(begin(), end(), tmp.Get());
                vector_.swap(tmp);
                size_ = std::move(new_size);
        } else {
            if (new_size > size_) {
                Iterator it = vector_.Get() + size_;
                while (it != vector_.Get() + new_size) {
                    *it = std::move(Type());
                    ++it;
                }
                size_ = std::move(new_size);
            } else {
                size_ = std::move(new_size);
            }
        }
    }

    Iterator begin() noexcept {
        return vector_.Get();
    }

    Iterator end() noexcept {
        return (size_ == 0 ? begin() : vector_.Get() + size_);
    }

    ConstIterator begin() const noexcept {
        return vector_.Get();
    }

    ConstIterator end() const noexcept {
        return (size_ == 0 ? begin() : vector_.Get() + size_);
    }

    ConstIterator cbegin() const noexcept {
        return vector_.Get();
    }

    ConstIterator cend() const noexcept {
        return (size_ == 0 ? begin() : vector_.Get() + size_);
    }
private:
    array_ptr::ArrayPtr<Type> vector_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

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
