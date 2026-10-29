#include <iostream>
#include "DynamicArray.hpp"
#include "List.hpp"

#include <cstdlib>
#include <ctime>

DynamicArray createRandomArray(int count, int min_val = 0, int max_val = 100) {
	DynamicArray arr;
	std::srand(std::time(nullptr));
	for (int i = 0; i < count; i++) {
		arr.add(min_val + std::rand() % (max_val - min_val + 1));
	}
	return arr;
}

void insertSort(DynamicArray &arr) {
	int i, key;

	for (int j = 1; j < arr.size(); j++) {
		key = arr.find(j);
		i = j - 1;

		while (i >= 0 && arr.find(i) > key) {
			arr.remove(i + 1);
			arr.add(i + 1, arr.find(i));
			i--;
		}
		arr.remove(i + 1);
		arr.add(i + 1, key);
	}
}

int getMinrun(int n) {
	int r = 0;
	while (n >= 64) {
		r |= (n & 1);
		n >>= 1;
	}
	return n + r;
}

int main() {
	DynamicArray arr = createRandomArray(25);
	List<int[2]> runs;
	arr.print();
	int minRun = getMinrun(25);
	//std::cout << std::endl;
	//insertSort(arr);
	//arr.print();
	int index = 0;
	while (index < arr.size()) {
		DynamicArray run;
		int lIndex = index;
		int rIndex;
		run.add(arr.find(index));
		index++;
		if (index < arr.size()) {
			bool direction = run.find(0) <= arr.find(index);

			run.add(arr.find(index));
			index++;

			while (index < arr.size()) {
				if (direction && run.find(run.size() - 1) <= arr.find(index)) {
					run.add(arr.find(index));
				}
				else if (!direction && run.find(run.size() - 1) > arr.find(index)) {
					run.add(arr.find(index));
				}
				else {
					rIndex = index - 1;
					break;
				}
				index++;
			}
			int indexes[2] = { lIndex, rIndex };
			runs.add(indexes);
		}
		insertSort(run);

		while (run.size() < minRun && index < arr.size()) {
			run.add(arr.find(index));
			index++;
		}

		insertSort(run);
	}
	
}