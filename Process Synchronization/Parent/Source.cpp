#include <iostream>
#include <Windows.h>

using namespace std;

int main(int argc, char* argv[]) {
	//acquire boss handles
	HANDLE* hEvents = new HANDLE[3];
	hEvents[0] = CreateEvent(NULL, FALSE, FALSE, "Event0");
	hEvents[1] = CreateEvent(NULL, FALSE, FALSE, "Event1");
	hEvents[2] = CreateEvent(NULL, FALSE, FALSE, "Event4");
	HANDLE mutex = CreateMutex(NULL, FALSE, "Mutex");
	ReleaseMutex(mutex);
	cout << "Enter message A or B, e for exit\n";
	char symbol;
	while (true) { //Check whether the process is to be terminated using hEvents[2]
		WaitForSingleObject(mutex, INFINITE);
		cout << "Active\n";
		cin >> symbol;
		//send to boss
		if (symbol == 'A') {
			cout << "Sending new A message\n";
			SetEvent(hEvents[0]);
		}
		if (symbol == 'B') {
			cout << "Sending new B message\n";
			SetEvent(hEvents[1]);
		}
		else if (symbol == 'e') {
			SetEvent(hEvents[2]);
		}
		cout << "Enter message\n";
		ReleaseMutex(mutex);
	}
	return 0;
}