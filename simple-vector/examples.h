#pragma once

#include <string>

#include "simple_vector.h"

namespace examples_simple_vector {

using namespace simple_vector;

void Contsructor() {
    SimpleVector<std::string> words1 {"the", "frogurt", "is", "also", "cursed"};
    SimpleVector<std::string> words2(words1); // {"the", "frogurt", "is", "also", "cursed"}
    SimpleVector<std::string> words3(std::move(words2)); // {"the", "frogurt", "is", "also", "cursed"} move from word2
    SimpleVector<std::string> words4(5, "Mo"); // {"Mo", "Mo", "Mo", "Mo", "Mo"}
}

void EqualsOperator() {
    SimpleVector<std::string> words1 {"the", "frogurt", "is", "also", "cursed"};
    SimpleVector<std::string> words2; // {}
    SimpleVector<std::string> words3; // {}
    words2 = words1; // {"the", "frogurt", "is", "also", "cursed"}
    words3 = std::move(words2);
}

void PushBack() {
    SimpleVector<int> vector {1, 2, 3}; // {1, 2, 3}
    vector.PushBack(4); // {1, 2, 3, 4}
    SimpleVector<std::string> words({"Hello"}); // {"Hello"}
    std::string word("World");
    words.PushBack(std::move(word)); // {"Hello", "World"}
}

void Insert() {
    SimpleVector<int> vector {1, 2, 3}; // {1, 2, 3}
    auto it = vector.begin();
    vector.Insert(it, 10); // {10, 1, 2, 3}
    int number = 20;
    it = vector.begin();
    vector.Insert(it, std::move(number)); // {20, 10, 1, 2, 3}
}

void PopBack() {
    SimpleVector<int> vector {1, 2, 3}; // {1, 2, 3}
    vector.PopBack(); // {1, 2}
}

void Erase() {
    SimpleVector<int> vector {1, 2, 3}; // {1, 2, 3}
    auto it = vector.cbegin();
    vector.Erase(it); // {2, 3}
}

void Swap() {
    SimpleVector<std::string> words1(5, "Mo"); // {"Mo", "Mo", "Mo", "Mo", "Mo"}
    SimpleVector<std::string> words2 {"the", "frogurt", "is", "also", "cursed"};
    words1.swap(words2);
}

void GetSize() {
    SimpleVector<std::string> words(5, "Mo"); // {"Mo", "Mo", "Mo", "Mo", "Mo"}
    words.GetSize(); // 5
}

void Reserve() {
    SimpleVector<int> vector {1, 2, 3}; // {1, 2, 3}
    vector.Reserve(5); // {1, 2, 3, 0, 0}
}

void GetCapacity() {
    SimpleVector<int> vector {1, 2, 3}; // {1, 2, 3}
    vector.Reserve(5); // {1, 2, 3, 0, 0}
    vector.GetCapacity(); // 5
}

void IsEmpty() {
    SimpleVector<int> vector {1, 2, 3}; // {1, 2, 3}
    vector.IsEmpty(); // false
    vector.Clear();
    vector.IsEmpty(); // true
}

void Clear() {
    SimpleVector<std::string> words(5, "Mo"); // {"Mo", "Mo", "Mo", "Mo", "Mo"}
    words.GetSize(); // 5
    words.Clear();
    words.GetSize(); // 0
}

void Resize() {
    SimpleVector<std::string> words(3, "Mo"); // {"Mo", "Mo", "Mo"}
    words.Resize(5); // {"Mo", "Mo", "Mo", "", ""}
    words.GetCapacity(); // 5
    words.GetSize(); // 5
}

} // namespace examples_simple_vector
