#include <iostream>
#include <mutex>
#include <Windows.h>

using namespace std;

int main(int argc, char* argv[]) {
	//acquire boss handles
	HANDLE* hEvents = new HANDLE[3];
	hEvents[0] = CreateEvent(NULL, FALSE, FALSE, "Event2");
	hEvents[1] = CreateEvent(NULL, FALSE, FALSE, "Event3");
	hEvents[2] = CreateEvent(NULL, FALSE, FALSE, "Event4");
	HANDLE semaphore = CreateSemaphore(NULL, 2, 2, "Semaphore");
	//ReleaseSemaphore(semaphore, 1, NULL);
	cout << "Enter message C or D, e for exit\n";
	char symbol;
	while (true) { //Check whether the process is to be terminated using hEvents[2]
		WaitForSingleObject(semaphore, INFINITE);
		cout << "Active\n";
		cin >> symbol;
		//send to boss
		if (symbol == 'C') {
			cout << "Sending new C message\n";
			SetEvent(hEvents[0]);
		}
		else if (symbol == 'D') {
			cout << "Sending new D message\n";
			SetEvent(hEvents[1]);
		}
		else if (symbol == 'e') {
			SetEvent(hEvents[2]);
		}
		cout << "Enter message\n";
		ReleaseSemaphore(semaphore, 1, NULL);
	}
	return 0;
}