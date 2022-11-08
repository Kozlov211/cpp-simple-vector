#pragma once

#include <cstdlib>
#include <utility>

namespace array_ptr {

template <typename Type>
class ArrayPtr {
public:
    ArrayPtr() = default;

    explicit ArrayPtr(size_t size);

    explicit ArrayPtr(Type* raw_ptr) noexcept;

    ArrayPtr(const ArrayPtr&) = delete;

    ArrayPtr& operator=(const ArrayPtr&) = delete;

    ArrayPtr(ArrayPtr&& other) noexcept;

    ArrayPtr& operator=(ArrayPtr&& other) noexcept;

    ~ArrayPtr();

    [[nodiscard]] Type* Release() noexcept;

    Type& operator[](size_t index) noexcept;

    const Type& operator[](size_t index) const noexcept;

    explicit operator bool() const;

    Type* Get() const noexcept;

    void swap(ArrayPtr& other) noexcept;


private:
    Type* raw_ptr_ = nullptr;
};

template <typename Type>
ArrayPtr<Type>::ArrayPtr(size_t size) {
    raw_ptr_ = size > 0 ? new Type[size] : nullptr;
}

template <typename Type>
ArrayPtr<Type>::ArrayPtr(Type* raw_ptr) noexcept  : raw_ptr_(raw_ptr) {
    raw_ptr = nullptr;
};

template <typename Type>
ArrayPtr<Type>::ArrayPtr(ArrayPtr&& other) noexcept {
    raw_ptr_ = std::exchange(other.raw_ptr_, nullptr);
}

template <typename Type>
ArrayPtr<Type>& ArrayPtr<Type>::operator=(ArrayPtr<Type>&& other) noexcept {
    if (this != &other) {
        raw_ptr_ = std::exchange(other.raw_ptr_, nullptr);
    }
    return *this;
}

template <typename Type>
ArrayPtr<Type>::~ArrayPtr() {
    delete[] raw_ptr_;
}

template <typename Type>
Type* ArrayPtr<Type>::Release() noexcept {
    Type* tmp = raw_ptr_;
    raw_ptr_ = nullptr;
    return tmp;
}


template <typename Type>
Type& ArrayPtr<Type>::operator[](size_t index) noexcept {
    return *(raw_ptr_ + index);
}

template <typename Type>
const Type& ArrayPtr<Type>::operator[](size_t index) const noexcept {
    return *(raw_ptr_ + index);
}


template <typename Type>
ArrayPtr<Type>::operator bool() const {
    return (raw_ptr_ ? true : false);
}

template <typename Type>
Type* ArrayPtr<Type>::Get() const noexcept {
    return raw_ptr_;
}

template <typename Type>
void ArrayPtr<Type>::swap(ArrayPtr& other) noexcept {
    std::swap(raw_ptr_, other.raw_ptr_);
}

} // namespace array_ptr

