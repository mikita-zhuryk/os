#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

using namespace std;

int main() {
	cout << "Enter array length\n";
	int size;
	cin >> size;
	HANDLE readPipeHandle;
	HANDLE writePipeHandle;
	if (!CreatePipe(&readPipeHandle, &writePipeHandle, NULL, 4)) {
		return 0;
	}
	cout << (int)readPipeHandle << '\n';
	cout << (int)writePipeHandle << '\n';
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	char buffer[20];
	snprintf(buffer, 20, "%lld %lld", (long long)readPipeHandle, (long long)writePipeHandle);
	if (!CreateProcess("Simple.exe", buffer, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "Failed to create process\n";
		return 0;
	}
	vector<int> primes;
	int temp;
	int count = 0;
	char command = '\0';
	cout << "If you want to stop execution, enter E\n";
	while (command != 'E') {
		ReadFile(readPipeHandle, &temp, 4, NULL, NULL);
		if (temp != -1) {
			primes.push_back(temp);
			cout << "New prime: " << temp << '\n';
			cin >> command;
		}
		else {
			cout << "No more primes in the array\n";
			break;
		}
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(writePipeHandle);
	CloseHandle(readPipeHandle);
	system("pause");
	return 0;
}