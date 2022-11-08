#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>

#include "simple_vector.h"

namespace tests_simple_vector {

namespace domain {

inline simple_vector::SimpleVector<int> GenerateVector(size_t size) {
    simple_vector::SimpleVector<int> v(size);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

class MoveClass {
public:
    MoveClass()
        : MoveClass(5) {
    }
    MoveClass(size_t num)
        : x_(num) {
    }
    MoveClass(const MoveClass& other) = delete;
    MoveClass& operator=(const MoveClass& other) = delete;
    MoveClass(MoveClass&& other) {
        x_ = std::exchange(other.x_, 0);
    }
    MoveClass& operator=(MoveClass&& other) {
        x_ = std::exchange(other.x_, 0);
        return *this;
    }
    size_t GetX() const {
        return x_;
    }

private:
    size_t x_;
};

} // namespace domain

void AllTest();

void Constructor();

void Modifiers();

void PushBack();

void Insert();

void Erase();

void Resize();

void PopBack();

void Swap();

void Clear();

void ElementAccess();

void CapacityFunctions();

void Empty();

void Size();

void Reserve();

void Capacity();

void Iterators();

} // namespace tests_simple_vector


