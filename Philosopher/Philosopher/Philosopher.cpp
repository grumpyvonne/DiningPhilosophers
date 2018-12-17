// Philosopher.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <CommCtrl.h>
#include <stdlib.h>
#include <shlwapi.h>
#include <mutex>
#include <iostream>
#include <sstream>
#include <fstream>


using namespace std;

HANDLE leftFork, rightFork;
SYSTEMTIME st;
ofstream logFile;


void log(char fileName[], char action[]) {
	if (logFile.is_open()) {
		logFile.close();
	}
	logFile.open(fileName, std::ofstream::app);
	char t[100];
	GetLocalTime(&st);
	sprintf_s(t, "%02d:%02d:%02d.%05d",
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);
	logFile << t << " --- " << action << endl;
	if (logFile.is_open()) {
		logFile.close();
	}
}


int main(int argc, char *argv[])
{
	stringstream strValue;
	strValue << argv[1];
	int philID;
	strValue >> philID;
	srand(time(0));
	char fileName[50];
	sprintf_s(fileName, "philosopher%d.txt", philID);	

		if (philID == 1) {
			leftFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork2"));
			rightFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork1"));
		}
		else if (philID == 2) {
			leftFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork3"));
			rightFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork2"));
		}
		else if (philID == 3) {
			leftFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork4"));
			rightFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork3"));
		}
		else if (philID == 4) {
			leftFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork5"));
			rightFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork4"));
		}
		else if (philID == 5) {
			leftFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork1"));
			rightFork = OpenMutex(MUTEX_ALL_ACCESS, TRUE, TEXT("Fork5"));
		}

		bool pickedLeft = false;
		bool pickedRight = false;

		Sleep(500 + (rand() * (int)(6000 - 500) / RAND_MAX));
		printf("Phil %d is hungry\n", philID);
		log(fileName, "Philosopher is hungry.");

		while (!pickedRight) {
			if (WaitForSingleObject(leftFork, 50) == WAIT_OBJECT_0) {
				pickedLeft = true;
				log(fileName, "Picked up left fork.");
			}
			else {
				pickedLeft = false;
				log(fileName, "Unable to pick up left fork.");
				Sleep(500 + (rand() * (int)(2000 - 500) / RAND_MAX));
			}

			if (pickedLeft) {
				if (WaitForSingleObject(rightFork, 50) == WAIT_OBJECT_0) {
					pickedRight = true;
					log(fileName, "Picked up right fork.");
					printf("Phil %d picked up left and right fork\n", philID);
					printf("Phil %d is eating\n", philID);
					log(fileName, "Is eating.");
					int timeEat = 1000 + (rand() * (int)(5000 - 1000) / RAND_MAX);
					Sleep(timeEat);
				}
				else {
					log(fileName, "Unable to pick up right fork; put down left fork.");
					pickedLeft = false;
					ReleaseMutex(leftFork);
					Sleep(500 + (rand() * (int)(2000 - 500) / RAND_MAX));
				}
			}
		}

		if (pickedRight) {
			ReleaseMutex(rightFork);
			CloseHandle(rightFork);
			log(fileName, "Put down right fork.");
		}
		if (pickedLeft) {
			ReleaseMutex(leftFork);
			CloseHandle(leftFork);
			log(fileName, "Put down left fork.");
		}
		log(fileName, "Has eaten; thinking.");
		printf("Phil %d has eaten.\n", philID);

}