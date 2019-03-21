#include <process.h>
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

string buildCommand() {
	size_t size;
	cout << "Enter array size\n";
	cin >> size;
	int* arr = new int[size];
	cout << "Enter array elements\n";
	for (size_t i = 0; i < size; ++i) {
		cin >> arr[i];
	}
	string s = "Child.exe ";
	s += to_string(size);
	for (size_t i = 0; i < size; ++i) {
		s.append(" ");
		s.append(to_string(arr[i]));
	}
	return s;
}

void runNormally() {
	cout << "First of all run normally\n";
	string s = buildCommand();
	cout << "Command:\n" << s << endl;
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION procInfo;

	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));

	if (CreateProcess(NULL, (LPSTR)s.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &procInfo)) {
		WaitForSingleObject(procInfo.hProcess, INFINITE);
		CloseHandle(procInfo.hThread);
		CloseHandle(procInfo.hProcess);
	}
	else {
		cout << "Error\n";
	}
}

void terminatePr() {
	cout << "Then terminate\n";
	string s = buildCommand();
	cout << "Command:\n" << s << endl;
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION procInfo;

	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));

	if (CreateProcess(NULL, (LPSTR)s.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &procInfo)) {
		if (TerminateProcess(procInfo.hProcess, 0)) {
			cout << "Terminated successfully\n";
		}
		else {
			cout << "Failed to terminate child process\n";
		}
		CloseHandle(procInfo.hThread);
		CloseHandle(procInfo.hProcess);
	}
	else {
		cout << "Error\n";
	}
}

int main() {
	cout << "Parent process started its work\n";
	runNormally();
	terminatePr();
	cout << "Parent process ended its work\n";
	system("pause");
	ExitProcess(0);
	return 0;
}