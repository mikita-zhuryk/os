#include <Windows.h>
#include <iostream>

using namespace std;

CRITICAL_SECTION cs_console_input;
CRITICAL_SECTION cs_console_output;
CRITICAL_SECTION cs_array_access;
CRITICAL_SECTION cs_sum;
HANDLE start_sum_event;

struct Data {

	uint32_t* arr;
	size_t size;
	uint16_t k;
	uint64_t result;
};

bool checkPrime(uint32_t n) {
	bool prime = true;
	for (uint32_t i = 2; i <= ceil(sqrt(n)); ++i) {
		if (n % i == 0) {
			prime = false;
		}
	}
	return prime;
}

void processArray(uint32_t* arr, size_t size, uint16_t delay, uint16_t k) {
	size_t count = 0;
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = i; j < size; ++j) {
			if (checkPrime(arr[j]) && !checkPrime(arr[i])) {
				swap(arr[i], arr[j]);
				break;
			}
		}
		Sleep(delay);
		if (i == k) {
			LeaveCriticalSection(&cs_array_access);
		}
	}
}

DWORD WINAPI work_thread(void* data) {
	EnterCriticalSection(&cs_array_access);
	uint16_t delay;
	EnterCriticalSection(&cs_console_output);
	cout << "Enter delay after each element in ms\n";
	LeaveCriticalSection(&cs_console_output);
	EnterCriticalSection(&cs_console_input);
	cin >> delay;
	LeaveCriticalSection(&cs_console_input);
	Data* d = (Data*)data;
	uint32_t* arr = d->arr;
	size_t size = d->size;
	uint16_t k = d->k;
	processArray(arr, size, delay, k);
	return 0;
}

DWORD WINAPI sum_thread(void* data) {
	EnterCriticalSection(&cs_sum);
	WaitForSingleObject(start_sum_event, INFINITE);
	Data* d = (Data*)data;
	d->result = 0;
	for (size_t i = 0; i < d->k; ++i) {
		d->result += d->arr[i];
	}
	LeaveCriticalSection(&cs_sum);
	return 0;
}

void printToConsole(uint32_t* arr, size_t size) {
	cout << "Array size: " << size << endl << "Array elements:\n";
	for (size_t i = 0; i < size; ++i) {
		cout << arr[i] << ' ';
	}
	cout << endl;
}

int main() {
	InitializeCriticalSection(&cs_console_input);
	InitializeCriticalSection(&cs_console_output);
	InitializeCriticalSection(&cs_array_access);
	InitializeCriticalSection(&cs_sum);
	start_sum_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	ios_base::sync_with_stdio(false);
	size_t n;
	cout << "Enter array length\n";
	cin >> n;
	uint32_t* arr = new uint32_t[n];
	cout << "Enter array elements\n";
	for (size_t i = 0; i < n; ++i) {
		cin >> arr[i];
	}
	printToConsole(arr, n);
	Data data;
	data.arr = arr;
	data.size = n;
	unsigned work_id;
	HANDLE hWorkThread = CreateThread(NULL, 2 * 1048576, work_thread, &data, CREATE_SUSPENDED, (LPDWORD)&work_id);
	int k;
	cout << "Enter k\n";
	cin >> k;
	data.k = k;
	ResumeThread(hWorkThread);
	Sleep(1);
	unsigned sum_id;
	HANDLE hSumThread = CreateThread(NULL, 2 * 1048576, sum_thread, &data, NULL, (LPDWORD)&sum_id);
	EnterCriticalSection(&cs_array_access);
	EnterCriticalSection(&cs_console_output);
	cout << "Array after sorting k elements\n";
	for (size_t i = 0; i < data.size; ++i) {
		cout << data.arr[i] << ' ';
	}
	cout << endl;
	LeaveCriticalSection(&cs_console_output);
	SetEvent(start_sum_event);
	LeaveCriticalSection(&cs_array_access);
	EnterCriticalSection(&cs_sum);
	EnterCriticalSection(&cs_console_output);
	cout << "Sum of the first " << k << " prime numbers in the array: " << data.result << endl;
	LeaveCriticalSection(&cs_console_output);
	LeaveCriticalSection(&cs_sum);
	CloseHandle(hWorkThread);
	CloseHandle(hSumThread);
	CloseHandle(start_sum_event);
	system("pause");
	return 0;
}