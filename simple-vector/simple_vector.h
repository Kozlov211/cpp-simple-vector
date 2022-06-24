#pragma once

#include <cassert>
#include <initializer_list>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include "array_ptr.h"
#include <iostream>
#include <utility>
#include <iterator>


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

	// Создаёт вектор из size элементов, инициализированных значением по умолчанию
	explicit SimpleVector(size_t size) : size_(size), capacity_(size) {
		ArrayPtr<Type> tmp(new Type[size_]{});
		vector_.swap(tmp);
	}

	// Конструктор копирования
	SimpleVector(const SimpleVector& other) {
		ArrayPtr<Type> tmp(new Type[other.capacity_]{});
		for (size_t i = 0; i < other.size_; ++i) {
	    	tmp[i] = other[i];
		}
		vector_.swap(tmp);
		size_ = other.size_;
		capacity_ = other.capacity_;
	}

	// Оператор присваивания
	SimpleVector& operator=(const SimpleVector& rhs)  {
		if (this != &rhs) {
			ArrayPtr<Type> tmp(new Type[rhs.capacity_]{});
			for (size_t i = 0; i < rhs.size_; ++i) {
				tmp[i] = rhs[i];
			}
			vector_.swap(tmp);
			size_ = rhs.size_;
			capacity_ = rhs.capacity_;
		}
		return *this;
	}

	// Конструктор перемещения
	SimpleVector(SimpleVector&& other) noexcept {
		vector_.swap(other.vector_);
		size_ = std::exchange(other.size_, 0);
		capacity_ = std::exchange(other.capacity_, 0);
	};

	// Перемещающий оператор
	SimpleVector& operator=(SimpleVector&& other) noexcept {
		if (this != &other) {
			vector_.swap(other.vector_);
			size_ = std::exchange(other.size_, 0);
			capacity_ = std::exchange(other.capacity_, 0);
		}
		return *this;
	}

	// Добавляет элемент в конец вектора
	// При нехватке места увеличивает вдвое вместимость вектора
	void PushBack(const Type& item) {
		Insert(end(), item);
	}
	
	// Добавляет элемент в конец вектора
	// При нехватке места увеличивает вдвое вместимость вектора
	void PushBack(Type&& item) {
		Insert(end(), std::move(item));
	} 

	// Вставляет значение value в позицию pos.
	// Возвращает итератор на вставленное значение
	// Если перед вставкой значения вектор был заполнен полностью,
	// вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
	Iterator Insert(ConstIterator pos, const Type& value) {
		if (capacity_ == 0) {
			ArrayPtr<Type> tmp(new Type[1]{});
			tmp[0] = value;
			vector_.swap(tmp);
			capacity_ = 1;
			size_ = 1;
			return begin();
		} else if (size_ == capacity_) {
			ArrayPtr<Type> tmp(new Type[capacity_ * 2]{});
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

	// Вставляет значение value в позицию pos.
	// Возвращает итератор на вставленное значение
	// Если перед вставкой значения вектор был заполнен полностью,
	// вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
	Iterator Insert(ConstIterator pos, Type&& value) {
		if (capacity_ == 0) {
		    ArrayPtr<Type> tmp(new Type[1]{});
		    tmp[0] = std::move(value);
		    vector_.swap(tmp);
		    capacity_ = 1;
		    size_ = 1;
		    return begin();
		} else if (size_ == capacity_) {
		    ArrayPtr<Type> tmp(new Type[capacity_ * 2]{});
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

	// "Удаляет" последний элемент вектора. Вектор не должен быть пустым
	void PopBack() noexcept {
		if (!IsEmpty()) {
		    --size_ ;
		}
	}

	// Удаляет элемент вектора в указанной позиции
	Iterator Erase(ConstIterator pos) {
		size_t index_pos = pos - cbegin();
		Iterator old_pos = begin() + index_pos;
		std::move(old_pos + 1, end(), old_pos);
		--size_;
		return old_pos;
	}

	// Обменивает значение с другим вектором
	void swap(SimpleVector& other) noexcept {
		vector_.swap(other.vector_);
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
	}

	// Создаёт вектор из size элементов, инициализированных значением value
	SimpleVector(size_t size, const Type& value) : size_(size), capacity_(size) {
		ArrayPtr<Type> tmp(new Type[size_]{});
		vector_.swap(tmp);
		std::fill(begin(), end(), value);
	}

	// Создаёт вектор из std::initializer_list
	SimpleVector(std::initializer_list<Type> init) : size_(init.size()), capacity_(init.size())  {
		ArrayPtr<Type> tmp(new Type[size_]{});
		vector_.swap(tmp);
		size_t count = 0;
		for (auto value : init) {
		    vector_[count++] = value;
		}
	}
	
	// Создание вектора с заданной вместимостью
	SimpleVector(ReserveProxyObj obj_capacity) {
		capacity_ = obj_capacity.Capacity();
	}

	// Возвращает количество элементов в массиве
	size_t GetSize() const noexcept {
		return size_;
	}
	
	// Увеличивает вместимость вектора
	void Reserve(size_t new_capacity) {
	if (new_capacity > capacity_) {
		    ArrayPtr<Type> tmp(new Type[new_capacity]{});
		    std::copy(begin(), end(), tmp.Get());
		    vector_.swap(tmp);
		    capacity_ = new_capacity;
		}
	}

	// Возвращает вместимость массива
	size_t GetCapacity() const noexcept {
		return capacity_;
	}

	// Сообщает, пустой ли массив
	bool IsEmpty() const noexcept {
		return (size_ == 0 ? true : false);
	}

	// Возвращает ссылку на элемент с индексом index
	Type& operator[](size_t index) noexcept {
		return vector_[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	const Type& operator[](size_t index) const noexcept {
		return vector_[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
	Type& At(size_t index) {
		if (index >= size_) {
			throw std::out_of_range("out_of_range");
		}
		return vector_[index];
	}

	// Возвращает константную ссылку на элемент с индексом index
	// Выбрасывает исключение std::out_of_range, если index >= size
	const Type& At(size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("out_of_range");
		}
		return vector_[index];
	}

	// Обнуляет размер массива, не изменяя его вместимость
	void Clear() noexcept {
		size_ = 0;
	}

	// Изменяет размер массива.
	// При увеличении размера новые элементы получают значение по умолчанию для типа Type
	void Resize(size_t new_size) {
		if (new_size > capacity_) {
				capacity_ = new_size;
				ArrayPtr<Type> tmp(new Type[capacity_]{});
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

	// Возвращает итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	Iterator begin() noexcept {
		return vector_.Get();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	Iterator end() noexcept {
		return (size_ == 0 ? begin() : vector_.Get() + size_);
	}

	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator begin() const noexcept {
		return vector_.Get();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator end() const noexcept {
		return (size_ == 0 ? begin() : vector_.Get() + size_);
	}

	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cbegin() const noexcept {
		return vector_.Get();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cend() const noexcept {
		return (size_ == 0 ? begin() : vector_.Get() + size_);
	}
private:
	ArrayPtr<Type> vector_;
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
