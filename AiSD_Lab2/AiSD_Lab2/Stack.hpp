#include <iostream>
#include "List.hpp"

template <typename T>
struct Stack {

	List<T> list = List<T>();

	void push(T elem) {
		list.add(0, elem);
	}

	T pop() {
		return list.remove(0);
	}

	int size() {
		return list.size();
	}

	T top(int depth) {
		return list.find(depth);
	}

	bool empty() {
		return list.size() == 0;
	}

	void print() {
		list.print();
	}
};