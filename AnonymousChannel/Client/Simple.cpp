#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <ctime>
#pragma warning(disable: 4996)

using namespace std;

bool checkPrime(int n) {
	for (int i = 2; i <= sqrt(n); ++i) {
		if (n % i == 0) { return false; }
	}
	return true;
}

int main(int argc, string argv[]) {
	srand(time(NULL));
	int size;
	cout << argc << '\n';
	char** unused = NULL;
	const char* str1 = argv[0].c_str();
	cout << str1 << '\n';
	HANDLE readHandle;
	readHandle = (HANDLE)strtoll(str1, unused, 10);
	const char* str2 = argv[1].c_str();
	cout << str2 << '\n';
	HANDLE writeHandle;
	writeHandle = (HANDLE)strtoll(str2, unused, 10);
	cout << (int)readHandle << ' ' << (int)writeHandle << '\n';
	ReadFile(readHandle, &size, 4, NULL, NULL);
	vector<__int32> vec(size);
	for (int i = 0; i < size; ++i) {
		vec[i] = rand() % 108;
		cout << "Element #i equals " << vec[i] << '\n';
	}
	for (int i = 0; i < size; ++i) {
		if (checkPrime(vec[i])) {
			WriteFile(writeHandle, &vec[i], 4, NULL, NULL);
		}
	}
	int end = -1;
	WriteFile(writeHandle, &end, 4, NULL, NULL);
	return 0;
}