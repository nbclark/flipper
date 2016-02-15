// FlipperUtils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 0)
	{
		WCHAR* wzLower = wcslwr(argv[0]);

		if (0 == wcscmp(L"lock", wzLower))
		{
			SHDeviceLockAndPrompt();
		}
		else if (0 == wcscmp(L"flightmode", wzLower))
		{
		}
		else if (0 == wcscmp(L"backlighton", wzLower))
		{
		}
		else if (0 == wcscmp(L"photo", wzLower))
		{
			SHCAMERACAPTURE capture = {0};
			SHCameraCapture(&capture);
		}
		else if (0 == wcscmp(L"backlighton", wzLower))
		{
		}
		else if (0 == wcscmp(L"backlightoff", wzLower))
		{
		}
		else if (0 == wcscmp(L"profilering", wzLower))
		{
		}
		else if (0 == wcscmp(L"profilevibrate", wzLower))
		{
		}
		else if (0 == wcscmp(L"profileoff", wzLower))
		{
		}
		else if (0 == wcscmp(L"type", wzLower))
		{
			if (argc > 1)
			{
				
				int uiLen = wcslen(argv[1]);

				UINT uiKeyState[1];
				UINT uiKeyDown[1];
				UINT KeyStateDownFlag= 0x0080;
				UINT KeyShiftDeadFlag= 0x20000;

				for (int i = 0; i < uiLen; ++i)
				{
					uiKeyDown[0] = (UINT)argv[1][i];
					uiKeyState[0] = KeyStateDownFlag;
					PostKeybdMessage((HWND)-1, NULL, KeyStateDownFlag, 1, &uiKeyState[0], &uiKeyDown[0]);
					uiKeyState[0] = KeyShiftDeadFlag;
					PostKeybdMessage((HWND)-1, NULL, KeyShiftDeadFlag, 1, &uiKeyState[0], &uiKeyDown[0]);
				}
			}
		}
	}
	return 0;
}

