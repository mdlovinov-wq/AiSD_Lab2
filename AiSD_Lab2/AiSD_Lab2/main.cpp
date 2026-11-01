#include <iostream>
#include "DynamicArray.hpp"
#include <array>
#include "Stack.hpp"
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>


void insertSort(DynamicArray<double> &arr) {
	int i;
	double key;

	for (int j = 1; j < arr.size(); j++) {
		key = arr.find(j);
		i = j - 1;

		while (i >= 0 && arr.find(i) > key) {
			arr.data[i + 1] = arr.find(i);
			i--;
		}
		arr.data[i + 1] = key;
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

void reverseRun(DynamicArray<double>& run) {
	int left = 0;
	int right = run.size() - 1;

	while (left < right) {
		double leftValue = run.find(left);
		double rightValue = run.find(right);

		run.data[left] = rightValue;
		run.data[right] = leftValue;

		left++;
		right--;
	}
}

void merge(DynamicArray<double>& leftRun, DynamicArray<double>& rightRun, DynamicArray<double>& resultRun) {
	resultRun.clear();
	int lIndex = 0;
	int rIndex = 0;
	while (lIndex < leftRun.size() && rIndex < rightRun.size()) {
		if (leftRun.find(lIndex) <= rightRun.find(rIndex)) {
			resultRun.add(leftRun.find(lIndex));
			lIndex++;
		}
		else {
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

DynamicArray<double> interval(DynamicArray<double>& arr, int lIndex, int rIndex) {
	DynamicArray<double> newArray;
	for (int i = lIndex; i <= rIndex; i++) {
		newArray.add(arr.find(i));
	}
	return newArray;
}

int upperBound(DynamicArray<double>& arr, int left, int right, double value) {
	while (left < right) {
		int mid = left + (right - left) / 2;

		if (arr.find(mid) <= value) {
			left = mid + 1;
		}
		else {
			right = mid;
		}
	}

	return left;
}

int lowerBound(DynamicArray<double>& arr, int left, int right, double value) {
	while (left < right) {
		int mid = left + (right - left) / 2;

		if (arr.find(mid) < value) {
			left = mid + 1;
		}
		else {
			right = mid;
		}
	}

	return left;
}

int main() {
	std::string line;
	std::getline(std::cin, line);

	DynamicArray<double> arr;

	std::istringstream input(line);
	std::string token;

	while (input >> token) {
		try {
			int pos = 0;

			if (token[0] == '-') {
				pos = 1;
			}

			bool dot = false;
			int digitsBefore = 0;
			int digitsAfter = 0;

			for (int i = pos; i < token.size(); i++) {
				if (token[i] >= '0' && token[i] <= '9') {
					if (dot) digitsAfter++;
					else digitsBefore++;
				}
				else if (token[i] == '.' && !dot) {
					dot = true;
				}
				else {
					throw std::invalid_argument("Invalid number");
				}
			}

			if (digitsBefore == 0 || (dot && digitsAfter == 0)) {
				throw std::invalid_argument("Invalid number");
			}

			double number = std::stod(token);

			if (!std::isfinite(number)) {
				throw std::invalid_argument("Invalid number");
			}

			arr.add(number);
		}
		catch (...) {
			std::cerr << "Error: unknown token: " << token << '\n';
			return 1;
		}
	}

	std::cout << line << "\n\n";
	if (arr.size() == 0) {
		return 0;
	}

	DynamicArray<std::array<int, 2>> runs;
	int minRun = getMinrun(arr.size());
	int index = 0;
	while (index < arr.size()) {
		DynamicArray<double> run;
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
			arr.data[lIndex + i] = run.find(i);
		}
		rIndex = index - 1;
		runs.add({ lIndex, rIndex });
	}


	Stack<std::array<int, 2>> stack;
	int runsIndex = 0;
	stack.push({ runs.find(runsIndex)[0], runs.find(runsIndex)[1] - runs.find(runsIndex)[0] + 1});
	runsIndex++;

	DynamicArray<double> intervalFirstRun, intervalSecondRun, mergeRun;
	std::array<int, 2> X, Y, Z;

	while (runsIndex < runs.size()) {
		stack.push({ runs.find(runsIndex)[0], runs.find(runsIndex)[1] - runs.find(runsIndex)[0] + 1} );
		while (stack.size() >= 2) {
			X = stack.top(0);
			Y = stack.top(1);
			if (stack.size() == 2) {
				if (Y[1] > X[1]) {
					break;
				}
				intervalFirstRun = interval(arr, Y[0], Y[0] + Y[1] - 1);
				intervalSecondRun = interval(arr, X[0], X[0] + X[1] - 1);
				merge(intervalFirstRun, intervalSecondRun, mergeRun);

				for (int i = 0; i < mergeRun.size(); i++) {
					arr.data[Y[0] + i] = mergeRun.find(i);
				}
				stack.pop();
				stack.pop();
				stack.push({ Y[0], mergeRun.size() });
			}
			else {
				Z = stack.top(2);
				if (Z[1] > X[1] + Y[1] && Y[1] > X[1]) {
					break;
				}

				if (X[1] < Z[1]) {
					intervalFirstRun = interval(arr, Y[0], Y[0] + Y[1] - 1);
					intervalSecondRun = interval(arr, X[0], X[0] + X[1] - 1);
					merge(intervalFirstRun, intervalSecondRun, mergeRun);
					for (int i = 0; i < mergeRun.size(); i++) {
						arr.data[Y[0] + i] = mergeRun.find(i);
					}
					stack.pop();
					stack.pop();
					stack.push({ Y[0], mergeRun.size() });
				}
				else {
					intervalFirstRun = interval(arr, Z[0], Z[0] + Z[1] - 1);
					intervalSecondRun = interval(arr, Y[0], Y[0] + Y[1] - 1);
					merge(intervalFirstRun, intervalSecondRun, mergeRun);
					for (int i = 0; i < mergeRun.size(); i++) {
						arr.data[Z[0] + i] = mergeRun.find(i);
					}
					stack.pop();
					stack.pop();
					stack.pop();
					stack.push({ Z[0], mergeRun.size() });
					stack.push(X);
				}
			}
		}

		runsIndex++;
	}

	bool leftIsSmaller;
	DynamicArray<double> resultArray;
	int tempIndex, bigIndex, bigEnd;
	while (stack.size() > 1) {
		resultArray.clear();
		X = stack.pop();
		Y = stack.pop();
		leftIsSmaller = Y[1] <= X[1];
		DynamicArray<double> tempArray;

		if (leftIsSmaller) {
			for (int i = 0; i < Y[1]; i++) {
				tempArray.add(arr.find(Y[0] + i));
			}

			tempIndex = 0;
			bigIndex = X[0];
			bigEnd = X[0] + X[1];

			int tempWins = 0;
			int bigWins = 0;
			const int minGallop = 7;

			while (tempIndex < tempArray.size() && bigIndex < bigEnd) {
				if (tempArray.find(tempIndex) <= arr.find(bigIndex)) {
					resultArray.add(tempArray.find(tempIndex++));
					tempWins++;
					bigWins = 0;
				}
				else {
					resultArray.add(arr.find(bigIndex++));
					bigWins++;
					tempWins = 0;
				}

				if (tempWins >= minGallop && tempIndex < tempArray.size() && bigIndex < bigEnd) {

					int boundary = upperBound(tempArray, tempIndex, tempArray.size(), arr.find(bigIndex));

					while (tempIndex < boundary) {
						resultArray.add(tempArray.find(tempIndex++));
					}

					tempWins = 0;
				}

				if (bigWins >= minGallop && tempIndex < tempArray.size() && bigIndex < bigEnd) {

					int boundary = lowerBound(arr, bigIndex, bigEnd, tempArray.find(tempIndex));

					while (bigIndex < boundary) {
						resultArray.add(arr.find(bigIndex++));
					}

					bigWins = 0;
				}
			}

			while (tempIndex < tempArray.size()) {
				resultArray.add(tempArray.find(tempIndex++));
			}

			while (bigIndex < bigEnd) {
				resultArray.add(arr.find(bigIndex++));
			}
		}
		else {
			for (int i = 0; i < X[1]; i++) {
				tempArray.add(arr.find(X[0] + i));
			}

			tempIndex = tempArray.size() - 1;
			bigIndex = Y[0] + Y[1] - 1;

			int tempWins = 0;
			int bigWins = 0;
			const int minGallop = 7;

			while (tempIndex >= 0 && bigIndex >= Y[0]) {

				if (arr.find(bigIndex) > tempArray.find(tempIndex)) {
					resultArray.add(arr.find(bigIndex));
					bigIndex--;

					bigWins++;
					tempWins = 0;
				}
				else {
					resultArray.add(tempArray.find(tempIndex));
					tempIndex--;

					tempWins++;
					bigWins = 0;
				}
				if (tempWins >= minGallop && tempIndex >= 0 && bigIndex >= Y[0]) {

					int boundary = lowerBound(tempArray, 0, tempIndex + 1, arr.find(bigIndex));
					while (tempIndex >= boundary) {
						resultArray.add(tempArray.find(tempIndex));
						tempIndex--;
					}

					tempWins = 0;
				}
				if (bigWins >= minGallop && tempIndex >= 0 && bigIndex >= Y[0]) {

					int boundary = upperBound(arr, Y[0], bigIndex + 1, tempArray.find(tempIndex));
					while (bigIndex >= boundary) {
						resultArray.add(arr.find(bigIndex));
						bigIndex--;
					}

					bigWins = 0;
				}
			}
			while (tempIndex >= 0) {
				resultArray.add(tempArray.find(tempIndex));
				tempIndex--;
			}

			while (bigIndex >= Y[0]) {
				resultArray.add(arr.find(bigIndex));
				bigIndex--;
			}
			reverseRun(resultArray);
		}

		for (int i = 0; i < resultArray.size(); i++) {
			arr.data[Y[0] + i] = resultArray.find(i);
		}
		stack.push({ Y[0], Y[1] + X[1] });
	}

	for (int i = 0; i < arr.size(); i++) {
		if (i > 0) {
			std::cout << ", ";
		}

		double number = arr.find(i);

		if (number == 0) {
			number = 0.0;
		}

		std::ostringstream output;
		output << std::fixed << std::setprecision(6) << number;

		std::string result = output.str();

		if (result.find('.') != std::string::npos) {
			while (result.back() == '0') {
				result.pop_back();
			}

			if (result.back() == '.') {
				result.pop_back();
			}
		}

		if (result == "-0") {
			result = "0";
		}

		std::cout << result;
	}

	std::cout << '\n';
	
}