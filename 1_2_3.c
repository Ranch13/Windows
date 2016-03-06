#include <Windows.h>
#include <stdio.h>
#define BUFSIZE	30

#define WINDOW_CLASS_NAME	("Sample Window Class")

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int counter = 0;
	char buffer[BUFSIZE];
	char dataBuffer[BUFSIZE];
	memset(dataBuffer, 0, BUFSIZE * sizeof(char));
	HANDLE hFile;
	int a = strlen(dataBuffer);
	DWORD dwBytesToWrite = (DWORD)BUFSIZE;
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;
	SYSTEMTIME lt;

	switch (msg)
	{
	case WM_RBUTTONDOWN:
		counter++;
		return 0;
	case WM_LBUTTONDOWN:
		sprintf(buffer, "%d", counter);
		SetWindowText(hWnd, TEXT(buffer));
		return 0;
	case WM_KEYDOWN:
		GetLocalTime(&lt);
		sprintf(dataBuffer, "The local time is : %02d:%02d\0", lt.wHour, lt.wMinute);
		if (wParam == VK_RETURN)
		{
			hFile = CreateFile("log.txt",// name of the file
				GENERIC_WRITE,          // open for writing
				0,                      // do not share
				NULL,                   // default security
				CREATE_NEW,             // create new file only
				FILE_ATTRIBUTE_NORMAL,  // normal file
				NULL);

			bErrorFlag = WriteFile(
				hFile,           // open file handle
				dataBuffer,      // start of data to write
				dwBytesToWrite,  // number of bytes to write
				&dwBytesWritten, // number of bytes that were written
				NULL);

			if (FALSE == bErrorFlag)
			{
				MessageBox(NULL, TEXT("Unable to write to file."), TEXT("Error"), MB_OK | MB_ICONWARNING);
				return -1;
			}

			else
			{
				if (dwBytesWritten != dwBytesToWrite)
				{
					MessageBox(NULL, TEXT("dwBytesWritten != dwBytesToWrite"), TEXT("Error"), MB_OK | MB_ICONWARNING);
					CloseHandle(hFile);
					return -1;
				}
			}

			CloseHandle(hFile);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdshow)
{
	WNDCLASS wc = { 0 };
	HWND hWnd = NULL;
	MSG msg = { 0 };
	char buffer[40]; /*for windows directory*/

	/* Fill the Window Class structure*/
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;

	/* Register the class (also check if the function failed) */
	if (FALSE == RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("Windwos Class Registartion Failed!"), TEXT("Error"), MB_OK | MB_ICONWARNING);
		return -1;
	}
	/*Create the window*/
	hWnd = CreateWindow(WINDOW_CLASS_NAME, TEXT("Sample Title"), WS_OVERLAPPEDWINDOW, 0, 0, 200, 200, NULL, NULL, hInstance, NULL);
	if (NULL == hWnd)
	{
		MessageBox(NULL, TEXT("Window creation failed!"), TEXT("Error"), MB_OK | MB_ICONWARNING);
		return -1;
	}
	/*The 3rd Ex. - MessageBox with windows installation folder*/
	GetSystemWindowsDirectory(buffer, sizeof(buffer));
	MessageBox(NULL, TEXT(buffer), TEXT("Information - 3rd Ex."), MB_OK | MB_ICONINFORMATION);
	/*Show the window*/
	ShowWindow(hWnd, nCmdshow);

	/*The message loop*/
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	/* When GetMessage receives the Wm_QUIT message, the wParam will indicate the required return code*/
	return msg.wParam;

}