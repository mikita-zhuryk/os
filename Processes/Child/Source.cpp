#include <iostream>
#include <algorithm>

using namespace std;

typedef unsigned int uint32;

bool checkPrime(uint32 n) {
	bool prime = true;
	for (uint32 i = 2; i <= ceil(sqrt(n)); ++i) {
		if (n % i == 0) {
			prime = false;
		}
	}
	return prime;
}

void processArray(uint32* arr, size_t size) {
	size_t count = 0;
	for (size_t i = 0; i < size; ++i) {
		if (checkPrime(arr[i])) {
			swap(arr[i], arr[count++]);
		}
	}
	for (size_t i = 0; i < size; ++i) {
		cout << arr[i] << ' ';
	}
	cout << endl;
}

uint32 main(int argc, char* argv[]) {
	cout << "Child process started its work\n";
	if (argc > 1) {
		size_t argNum = atoi(argv[1]);
		char** numbers = argv + 2;
		uint32* argArray = new uint32[argNum];
		for (size_t i = 0; i < argNum; ++i) {
			argArray[i] = atoi(numbers[i]);
		}
		processArray(argArray, argNum);
		delete[] argArray;
	}
	cout << "Child process ended its work\n";
	system("pause");
	return 0;
}