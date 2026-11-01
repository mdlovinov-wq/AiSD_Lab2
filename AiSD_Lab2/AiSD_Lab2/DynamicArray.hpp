#include <iostream>
#include <stdexcept>
#include <type_traits>

template <typename T>
struct DynamicArray {
	T* data = NULL;
	int _size = 0;
	int _capacity = 2;

	DynamicArray() {
		data = new T[_capacity];
	}

	DynamicArray(const DynamicArray& other) {
		_size = other._size;
		_capacity = other._capacity;

		data = new T[_capacity];

		for (int i = 0; i < _size; i++) {
			data[i] = other.data[i];
		}
	}

	DynamicArray& operator=(const DynamicArray& other) {
    if (this == &other) {
        return *this;
    }

    T* newData = new T[other._capacity];

    for (int i = 0; i < other._size; i++) {
        newData[i] = other.data[i];
    }

    delete[] data;

    data = newData;
    _size = other._size;
    _capacity = other._capacity;

    return *this;
}

	void add(T elem) {
		add(size(), elem);
	}

	void add(int index, T elem) {
		if (index < 0 || index > _size) {
			throw std::runtime_error("Out of bounds");
		}
		if (_size == _capacity) {
			resize();
		}
		for (int i = _size - 1; i >= index; i--) {
			data[i + 1] = data[i];
		}
		data[index] = elem;
		_size++;
	}

	void addArray(DynamicArray<T> array) {
		for (int i = 0; i < array.size(); i++) {
			add(array.find(i));
		}
 	}

	void remove(int index) {
		if (index < 0 || index >= _size) {
			throw std::runtime_error("Out of bounds");
		}

		for (int i = index; i < _size - 1; i++) {
			data[i] = data[i + 1];
		}
		_size--;
	}

	T find(int index) {
		if (index < 0 || index >= _size) {
			throw std::runtime_error("Out of bounds");
		}

		return data[index];
	}

	int size() {
		return _size;
	}

	void print() {
		for (int i = 0; i < _size; i++) {
			if constexpr (std::is_same_v<T, std::array<int, 2>>) {
				std::cout << data[i][0] << " "
					<< data[i][1] << '\n';
			}
			else {
				std::cout << data[i] << " ";
			}
		}
		std::cout << '\n';
	}

	bool isSorted() {
		for (int i = 0; i < size() - 1; i++) {
			if (data[i] > data[i + 1]) {
				return false;
			}
		}
		return true;
	}

	void clear() {
		_size = 0;
	}

	void resize() {
		_capacity *= 2;
		T* newData = new T[_capacity];

		for (int i = 0; i < _size; i++) {
			newData[i] = data[i];
		}
		delete[] data;
		data = newData;
	}

	~DynamicArray() {
		delete[] data;
	}

};
