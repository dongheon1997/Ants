#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = LPSTR("이게 이름인가");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = g_hInst = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

bool key[256] = { 0 };

bool block[11][15] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

int getLeftX(int x1, int x2, int y1, int y2)
{
}

int getRightX(int x1, int x2, int y1, int y2)
{
}

int getDownY(int x1, int x2, int y1, int y2)
{
	x1 /= 50;
	if (x2 % 50 == 0)
	{
		x2 /= 50;
		x2 -= 1;
	}
	else
	{
		x2 /= 50;
	}

	// x1 ~ x2 (x1 = x2 일 수 있음)

	if (y1 % 50 == 0)
	{
		y1 /= 50;
	}
	else
	{
		y1 /= 50;
		y1 += 1;
	}

	if (y2 % 50 == 0)
	{
		y2 /= 50;
	}
	else
	{
		y2 /= 50;
		y2 += 1;
	}

	// y1 ~ y2 (y1 = y2 일 수 있음)

	for (int i = y1; i < y2; i += 1)
	{
		for (int j = x1; j <= x2; j += 1)
		{
			if (block[i][j] == true) return i * 50;
		}
	}

	return 0;
}

static int x = 75;
static int y = 75;
static int ay = 0;
static int speed = 5;
static int width = 5;
static int hight = 20;
static bool isJump = true;

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (key[VK_LEFT]) x -= speed;
	if (key[VK_RIGHT]) x += speed;

	if (key[VK_UP] && !isJump)
	{
		ay -= 20;
		isJump = true;
	}

	y += ay;
	ay += 2;

	if (y >= 500)
	{
		y = 500;
		isJump = false;
		ay = 0;
	}

	InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 10, (TIMERPROC)TimerProc);
		return 0;
	case WM_KEYDOWN:
		key[wParam] = true;
		return 0;
	case WM_KEYUP:
		key[wParam] = false;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 11; i += 1)
		{
			for (int j = 0; j < 15; j += 1)
			{
				if (block[i][j]) Rectangle(hdc, j * 50, i * 50, j * 50 + 50, i * 50 + 50);
			}
		}
		TextOut(hdc, x, y, "A", 1);
		
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
