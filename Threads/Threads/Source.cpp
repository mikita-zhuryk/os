#include <ctime>
#include <iostream>
#include <Windows.h>
#include <process.h>
#pragma warning(disable:4996)

typedef unsigned int uint32;

struct Data {

	int* inputs;
	uint32 inputs_size;
	int x;
	int* filtered;
	uint32 filtered_size;

};

struct FibData {
	uint32* fib;
	uint32 size;
};

DWORD WINAPI sort(LPVOID param) {
	Data* data = (Data*)param;
	int x = data->x;
	uint32 n = data->inputs_size;
	uint32 count = 0;
	data->filtered = (int*)calloc(n, sizeof(int));
	Sleep(200);
	for (uint32 i = 0; i < n; ++i) {
		if (data->inputs[i] > x) {
			data->filtered[count++] = data->inputs[i];
		}
	}
	data->filtered_size = count;
	data->filtered = (int*)realloc(data->filtered, count * sizeof(int));
	return 0;
}

unsigned __stdcall Fib(void* d) {
	FibData* data = (FibData*)d;
	uint32 n = data->size;
	data->fib = new uint32[n];
	data->fib[0] = 1;
	data->fib[1] = 1;
	for (uint32 i = 2; i < n; ++i) {
		data->fib[i] = data->fib[i - 1] + data->fib[i - 2];
	}
	return 0;
}

void startThread(HANDLE& h, DWORD& id, Data& data) {
	h = CreateThread(
		NULL,
		16 * 1024 * 1024,
		sort,
		&data,
		0,
		&id);
}

void printArr(int* arr, uint32 size) {
	for (uint32 i = 0; i < size; ++i) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int* randomArray(uint32& n) {
	printf("Enter array size\n");
	scanf("%d", &n);
	int* arr = (int*)calloc(n, sizeof(int));
	for (uint32 i = 0; i < n; ++i) {
		arr[i] = rand() % 20;
	}
	printf("Input array:\n");
	printArr(arr, n);
	return arr;
}

int main() {
	srand(time(NULL));
	uint32 n = 0;
	int* arr = randomArray(n);
	int x = 0;
	printf("Enter X\n");
	scanf("%d", &x);

	Data data;
	data.inputs = arr;
	data.inputs_size = n;
	data.x = x;

	DWORD dt = 0;
	printf("Enter delay time in ms\n");
	scanf("%d", &dt);

	DWORD workerID;
	HANDLE hWorker;
	startThread(hWorker, workerID, data);
	SuspendThread(hWorker);
	Sleep(dt);
	ResumeThread(hWorker);

	DWORD exitCode;
	if (hWorker != NULL) {
		exitCode = WaitForSingleObject(hWorker, INFINITE);
		CloseHandle(hWorker);
		hWorker = NULL;
		printf("Elements greater than %d:\n", x);
		printArr(data.filtered, data.filtered_size);
	}
	free(data.inputs);
	free(data.filtered);

	uint32 count;
	printf("Enter number of Fibonacci numbers\n");
	scanf("%d", &count);
	FibData fd;
	fd.size = count;

	uint32 countID;
	HANDLE hCount = (HANDLE)_beginthreadex(NULL, 1024 * 1024, Fib, &fd, CREATE_SUSPENDED, &countID);
	ResumeThread(hCount);
	WaitForSingleObject(hCount, INFINITE);
	CloseHandle(hCount);

	for (uint32 i = 0; i < fd.size; ++i) {
		printf("%d ", fd.fib[i]);
	}
	printf("\n");

	system("pause");
	return 0;
}