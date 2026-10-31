#include <iostream>
#include "DynamicArray.hpp"
#include "List.hpp"
#include <array>
#include <cstdlib>
#include <ctime>
#include "Stack.hpp"

DynamicArray<int> createRandomArray(int count, int min_val = 0, int max_val = 100) {
	DynamicArray<int> arr;
	std::srand(std::time(nullptr));
	for (int i = 0; i < count; i++) {
		arr.add(min_val + std::rand() % (max_val - min_val + 1));
	}
	return arr;
}

void insertSort(DynamicArray<int> &arr) {
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

void reverseRun(DynamicArray<int>& run) {
	int left = 0;
	int right = run.size() - 1;

	while (left < right) {
		int leftValue = run.find(left);
		int rightValue = run.find(right);

		run.remove(left);
		run.add(left, rightValue);

		run.remove(right);
		run.add(right, leftValue);

		left++;
		right--;
	}
}

void merge(DynamicArray<int>& leftRun, DynamicArray<int>& rightRun, DynamicArray<int>& resultRun) {
	
	int lIndex = 0;
	int rIndex = 0;
	while (lIndex < leftRun.size() && rIndex < rightRun.size()) {
		if (leftRun.find(lIndex) > rightRun.find(rIndex)) {
			resultRun.add(leftRun.find(lIndex));
			lIndex++;
		}
		else if (leftRun.find(lIndex) <= rightRun.find(rIndex)) {
			resultRun.add(rightRun.find(rIndex));
			rIndex++;
		}
	}

	while (lIndex < leftRun.size()) {
		resultRun.add(leftRun.find(lIndex));
		lIndex++;
	}
	while (rIndex < rightRun.size()) {
		resultRun.add(rightRun.find(rIndex));
		rIndex++;
	}

}

DynamicArray<int> interval(DynamicArray<int> arr, int lIndex, int rIndex) {
	DynamicArray<int> newArray;
	for (int i = lIndex; i <= rIndex; i++) {
		newArray.add(arr.find(i));
	}
	return newArray;
}

int main() {
	DynamicArray<int> arr = createRandomArray(500);
	DynamicArray<std::array<int, 2>> runs;
	arr.print();
	int minRun = getMinrun(500);
	int index = 0;
	while (index < arr.size()) {
		DynamicArray<int> run;
		int lIndex = index;
		int rIndex;
		run.add(arr.find(index));
		index++;
		bool direction = true;
		if (index < arr.size()) {
			direction = run.find(0) <= arr.find(index);

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
					
					break;
				}
				index++;
			}
		}
		if (!direction) {
			reverseRun(run);
		}

		if (run.size() < minRun) 
		{
			while (run.size() < minRun && index < arr.size()) {
				run.add(arr.find(index));
				index++;
			}
			insertSort(run);
		}

		
		for (int i = 0; i < run.size(); i++) {
			arr.remove(lIndex + i);
			arr.add(lIndex + i, run.find(i));
		}
		rIndex = index - 1;
		runs.add({ lIndex, rIndex });
	}

	arr.print();
	std::cout << runs.size() << std::endl;

	Stack<std::array<int, 2>> stack;
	int runsIndex = 0;
	stack.push({ runs.find(runsIndex)[0], runs.find(runsIndex)[1] - runs.find(runsIndex)[0] + 1});
	runsIndex++;

	DynamicArray<int> intervalFirstRun, intervalSecondRun, mergeRun;
	bool isZ;
	std::array<int, 2> X, Y, Z;

	while (runsIndex < runs.size()) {
		stack.push({ runs.find(runsIndex)[0], runs.find(runsIndex)[1] - runs.find(runsIndex)[0] + 1} );
		if (stack.size() >= 2) {
			X = stack.top(0);
			Y = stack.top(1);
			if (!(Y[1] > X[1])) {
				intervalFirstRun = interval(arr, Y[0], Y[0] + Y[1] - 1);
				intervalSecondRun = interval(arr, X[0], X[0] + X[1] - 1);
				merge(intervalFirstRun, intervalSecondRun, mergeRun);
				for (int i = 0; i < mergeRun.size(); i++) {
					arr.remove(Y[0] + i);
					arr.add(Y[0] + i, mergeRun.find(i));
				}
				stack.pop();
				stack.pop();
				stack.push({ Y[0], mergeRun.size()});
			}
		}
	}
	
	
}