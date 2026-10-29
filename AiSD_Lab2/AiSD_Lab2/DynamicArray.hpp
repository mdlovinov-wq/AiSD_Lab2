#include <iostream>
#include <stdexcept>

struct DynamicArray {
	int* data = NULL;
	int _size = 0;
	int _capacity = 2;

	DynamicArray() {
		data = new int[_capacity];
	}

	void add(int elem) {
		add(size(), elem);
	}

	void add(int index, int elem) {
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

	void remove(int index) {
		if (index < 0 || index >= _size) {
			throw std::runtime_error("Out of bounds");
		}

		for (int i = index; i < _size - 1; i++) {
			data[i] = data[i + 1];
		}
		_size--;
	}

	int find(int index) {
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
			std::cout << data[i] << " ";
		}
	}

	void resize() {
		_capacity *= 2;
		int* newData = new int[_capacity];

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
