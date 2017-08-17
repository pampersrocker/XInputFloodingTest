// XInputFloodingTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <Xinput.h>

volatile bool RequestedExit = false;
volatile WORD Intensity = 0;
volatile DWORD InputUpdateRate = 33; // Default to 33ms
HANDLE IntensityMutex;

DWORD WINAPI InputLoop(void* data)
{
	while (!RequestedExit)
	{
		XINPUT_VIBRATION Vibration;
		WaitForSingleObject(IntensityMutex, INFINITE);
		Vibration.wLeftMotorSpeed = Intensity;
		Vibration.wRightMotorSpeed = Intensity;
		ReleaseMutex(IntensityMutex);
		for (DWORD ControllerId = 0; ControllerId < XUSER_MAX_COUNT; ++ControllerId)
		{
			DWORD Result = XInputSetState(ControllerId, &Vibration);
		}
		Sleep(InputUpdateRate);
	}
	return 0;
}

DWORD WINAPI UpdateLoop(void* data) 
{
	while (!RequestedExit)
	{
		WaitForSingleObject(IntensityMutex, INFINITE);
		Intensity = Intensity == 0 ? 32767 : 0;
		ReleaseMutex(IntensityMutex);
		// Sleep for 0.5s
		Sleep(500);
	}
	return 0;
}



int main()
{
	HANDLE ThreadHandles[2];
	IntensityMutex = CreateMutex(nullptr, false, nullptr);
	ThreadHandles[0] = CreateThread(nullptr, 0, UpdateLoop, nullptr, 0, nullptr);
	ThreadHandles[1] = CreateThread(nullptr, 0, InputLoop, nullptr, 0, nullptr);
	if (ThreadHandles[0] && ThreadHandles[1] && IntensityMutex)
	{
		printf("Update rate set to 33ms\n");

		while (!RequestedExit)
		{
			printf("Enter update rate in ms (enter negative to close):");
			int UpdateRate = 0;
			scanf("%d", &UpdateRate);
			if (UpdateRate < 0)
			{
				RequestedExit = true;
				WaitForMultipleObjects(2, ThreadHandles, true, INFINITE);
				CloseHandle(ThreadHandles[0]);
				CloseHandle(ThreadHandles[1]);
				CloseHandle(IntensityMutex);
			}
			else
			{
				InputUpdateRate = UpdateRate;
			}

		}
	}

    return 0;

}

