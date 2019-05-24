#include <iostream>
#include <thread>
#include <windows.h>
#include <process.h>
#include <stdio.h>
using namespace std;
HANDLE hEvent;
int main()
{

	int amountOfMessages;
	int amount = 0;
	int K;
	cout << "Input amountOfMessages: \n";
	cin >> amountOfMessages;
	cout << "Input amount: \n";
	cin >> amount;
	char data[100];
	char number[8];
	char *messeges = new char[amountOfMessages];
	_itoa_s(amountOfMessages, data, 10);
	int *arr = new int[amountOfMessages];
	cout << "Input array: \n";
	for (int i = 0; i < amountOfMessages; ++i)
	{
		cin >> messeges[i];
		arr[i] = 2;
		_itoa_s(arr[i], number, 10);
		strcat_s(data, " ");
		strcat_s(data, number);
	}
	cout << "Input number K: \n";
	cin >> K;
	_itoa_s(K, number, 10);
	strcat_s(data, " ");
	strcat_s(data, number);
	char lpszAppName[] = "C:\\Users\\kovic\\Desktop\\Project4\\Debug\\Employee.exe";
	STARTUPINFO *si = new STARTUPINFO[amount];
	PROCESS_INFORMATION *pi = new PROCESS_INFORMATION[amount];
	unsigned int id;
	hEvent = CreateEvent(NULL, false, false, NULL);
	for (int i = 0; i < amount; ++i)
	{
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		if (!CreateProcess(lpszAppName, data, NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i]))
		{
			cout << "wrong";
		}
	}
	system("pause");
	return 0;
}