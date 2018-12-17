#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <CommCtrl.h>
#include <process.h>
#include <stdlib.h>
#include <shlwapi.h>

using namespace std;


int main()
{
	HANDLE fork[5];
	HANDLE processes[5];
	
	STARTUPINFO si[5];
	PROCESS_INFORMATION pi[5];
	for (int i = 0; i < 5; i++) {
		ZeroMemory(&si[i], sizeof(si[i]));
		si[i].cb = sizeof(si[i]);
		ZeroMemory(&pi[i], sizeof(pi[i]));
	}
	
	fork[0] = CreateMutex(NULL, FALSE, TEXT("Fork1"));
	fork[1] = CreateMutex(NULL, FALSE, TEXT("Fork2"));
	fork[2] = CreateMutex(NULL, FALSE, TEXT("Fork3"));
	fork[3] = CreateMutex(NULL, FALSE, TEXT("Fork4"));
	fork[4] = CreateMutex(NULL, FALSE, TEXT("Fork5"));

	char path [50];
	for (int philID = 0; philID < 5; philID++) {
		sprintf_s(path, "Philosopher.exe %d", philID + 1);
		if (CreateProcess(NULL, path, NULL, NULL, FALSE, 0, NULL, NULL, &si[philID], &pi[philID])) {
			processes[philID] = pi[philID].hProcess;
		}
		else printf("CreateProcess %d (philosopher %d) failed. Error %d \n", philID, philID+1, GetLastError());
	}

	WaitForMultipleObjects(5, processes, TRUE, INFINITE);
	for (int philID = 0; philID < 5; philID++) {
		CloseHandle(pi[philID].hProcess);
		CloseHandle(pi[philID].hThread);
	}
	WaitForMultipleObjects(5, fork, TRUE, INFINITE);
	for (int num = 0; num < 5; num++) {
		CloseHandle(fork[num]);
	}
	system("pause");
	return 0;
}