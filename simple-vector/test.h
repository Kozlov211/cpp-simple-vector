#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <string>

#include "simple_vector.h"

class X {
public:
    X()
        : X(5) {
    }
    X(size_t num)
        : x_(num) {
    }
    X(const X& other) = delete;
    X& operator=(const X& other) = delete;
    X(X&& other) {
        x_ = std::exchange(other.x_, 0);
    }
    X& operator=(X&& other) {
        x_ = std::exchange(other.x_, 0);
        return *this;
    }
    size_t GetX() const {
        return x_;
    }

private:
    size_t x_;
};

namespace tests_simple_vector {

using namespace simple_vector;

SimpleVector<int> GenerateVector(size_t size) {
    SimpleVector<int> v(size);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

void TemporaryObjConstructor() {
    using namespace std;

    const size_t size = 1000000;
    SimpleVector<int> moved_vector(GenerateVector(size));
    assert(moved_vector.GetSize() == size);
}

void TemporaryObjOperator() {
    using namespace std;
    const size_t size = 1000000;
    SimpleVector<int> moved_vector;
    assert(moved_vector.GetSize() == 0);
    moved_vector = GenerateVector(size);
    assert(moved_vector.GetSize() == size);
}

void NamedMoveConstructor() {
    using namespace std;
    const size_t size = 1000000;
    SimpleVector<int> vector_to_move(GenerateVector(size));
    assert(vector_to_move.GetSize() == size);
    SimpleVector<int> moved_vector(move(vector_to_move));
    assert(moved_vector.GetSize() == size);
    assert(vector_to_move.GetSize() == 0);
}

void NamedMoveOperator() {
    using namespace std;
    const size_t size = 1000000;
    SimpleVector<int> vector_to_move(GenerateVector(size));
    assert(vector_to_move.GetSize() == size);
    SimpleVector<int> moved_vector = move(vector_to_move);
    assert(moved_vector.GetSize() == size);
    assert(vector_to_move.GetSize() == 0);
}

void NoncopiableMoveConstructor() {
    using namespace std;
    const size_t size = 5;
    SimpleVector<X> vector_to_move;
    for (size_t i = 0; i < size; ++i) {
        vector_to_move.PushBack(X(i));
    }
    SimpleVector<X> moved_vector = move(vector_to_move);
    assert(moved_vector.GetSize() == size);
    assert(vector_to_move.GetSize() == 0);
    for (size_t i = 0; i < size; ++i) {
        assert(moved_vector[i].GetX() == i);
    }
}

void NoncopiablePushBack() {
    using namespace std;
    const size_t size = 5;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }
    assert(v.GetSize() == size);
    for (size_t i = 0; i < size; ++i) {
        assert(v[i].GetX() == i);
    }
}

void NoncopiableInsert() {
    using namespace std;
    const size_t size = 5;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }
    v.Insert(v.begin(), X(size + 1));
    assert(v.GetSize() == size + 1);
    assert(v.begin()->GetX() == size + 1);
    v.Insert(v.end(), X(size + 2));
    assert(v.GetSize() == size + 2);
    assert((v.end() - 1)->GetX() == size + 2);
    v.Insert(v.begin() + 3, X(size + 3));
    assert(v.GetSize() == size + 3);
    assert((v.begin() + 3)->GetX() == size + 3);
}

void NoncopiableErase() {
    using namespace std;
    const size_t size = 3;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(X(i));
    }
    auto it = v.Erase(v.begin());
    assert(it->GetX() == 1);
}

void AllTest() {
    TemporaryObjConstructor();
    TemporaryObjOperator();
    NamedMoveConstructor();
    NamedMoveOperator();
    NoncopiableMoveConstructor();
    NoncopiablePushBack();
    NoncopiableInsert();
    NoncopiableErase();
}

} // namespace tests_simple_vector
