#include <Windows.h>
#include <iostream>
#include <string>
#pragma warning(disable: 4996)

using namespace std;

int main() {
	HANDLE* hEvents = new HANDLE[5];
	for (char i = 0; i < 5; ++i) {
		hEvents[i] = CreateEvent(NULL, FALSE, FALSE, (string("Event") + (char)(i + '0')).c_str());
	}
	//Create child processes
	int parentCount;
	int childCount;
	cout << "Enter number of parent processes\n";
	cin >> parentCount;
	cout << "Enter number of child processes\n";
	cin >> childCount;
	int messageNumber;
	cout << "How many messages to receive?\n";
	cin >> messageNumber;
	STARTUPINFO* parentSI = new STARTUPINFO[parentCount];
	PROCESS_INFORMATION* parentPI = new PROCESS_INFORMATION[parentCount];
	STARTUPINFO* childSI = new STARTUPINFO[childCount];
	PROCESS_INFORMATION* childPI = new PROCESS_INFORMATION[childCount];
	for (int i = 0; i < parentCount; ++i) {
		ZeroMemory(parentSI + i, sizeof(STARTUPINFO));
		parentSI[i].cb = sizeof(STARTUPINFO);
		ZeroMemory(parentPI + i, sizeof(PROCESS_INFORMATION));
		CreateProcess("Parent.exe", NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, parentSI + i, parentPI + i);
	}
	for (int i = 0; i < childCount; ++i) {
		ZeroMemory(childSI + i, sizeof(STARTUPINFO));
		childSI[i].cb = sizeof(STARTUPINFO);
		ZeroMemory(childPI + i, sizeof(PROCESS_INFORMATION));
		CreateProcess("Child.exe", NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, childSI + i, childPI + i);
	}
	//Process messages
	int eventNum;
	for (int i = 0; i < messageNumber; ++i) {
		eventNum = WaitForMultipleObjects(5, hEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (eventNum == 0) {
			cout << "Message A from process Parent\n";
			ResetEvent(hEvents[0]);
		}
		else if (eventNum == 1) {
			cout << "Message B from process Parent\n";
			ResetEvent(hEvents[1]);
		}
		else if (eventNum == 2) {
			cout << "Message C from process Child\n";
			ResetEvent(hEvents[2]);
		}
		else if (eventNum == 3) {
			cout << "Message D from process Child\n";
			ResetEvent(hEvents[3]);
		}
		else if (eventNum == 4) {
			cout << "Request to terminate parent and child processes\n";
			break;
		}
	}
	for (int i = 0; i < parentCount; ++i) {
		TerminateProcess(parentPI[i].hProcess, 0);
		CloseHandle(parentPI[i].hThread);
		CloseHandle(parentPI[i].hProcess);
	}
	for (int i = 0; i < childCount; ++i) {
		TerminateProcess(childPI[i].hProcess, 0);
		CloseHandle(childPI[i].hThread);
		CloseHandle(childPI[i].hProcess);
	}
	for (int i = 0; i < 5; ++i) {
		CloseHandle(hEvents[i]);
	}
	delete[] hEvents;
	delete[] parentPI;
	delete[] parentSI;
	delete[] childPI;
	delete[] childSI;
	system("pause");
	return 0;
}