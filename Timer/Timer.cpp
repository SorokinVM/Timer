#include <windows.h>
#include "KWnd.h"
#include <ctime>
#include <stdio.h>


HDC hDC;
time_t StartTime;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//====================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	KWnd mainWnd(L"Timer", hInstance, nCmdShow, WndProc);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rect;
	time_t CurrentTime, ElapsedTime;
	tm tp;
	errno_t err;
	wchar_t Result[] = L"00:00:00";
	size_t LenResult = 9;// sizeof(Result);
	HFONT hFontOriginal, hFontTime;
	int nHeight;

	switch (uMsg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);


		SetTimer(hWnd, 1, 1000, NULL);
		time(&StartTime);
		
		// скрыть заголовок окна
		//LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		//lpcs->style &= ~WS_CAPTION;
		//SetWindowLong(hWnd, GWL_STYLE, lpcs->style);
		//

		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		
		time(&CurrentTime);

		ElapsedTime = CurrentTime - StartTime;
		
		err = gmtime_s(&tp, &ElapsedTime);
		_snwprintf_s(Result, LenResult, L"%02u:%02u:%02u", tp.tm_hour, tp.tm_min, tp.tm_sec);

		nHeight = min(rect.bottom - rect.top,(rect.right-rect.left)/3.5); // 3.5 - опытным путем к ширине формата 00:00:00

		hFontTime = CreateFont(nHeight, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, NULL);
		hFontOriginal = (HFONT)SelectObject(hDC, hFontTime);

		DrawText(hDC, Result, LenResult, &rect, DT_SINGLELINE);
		//
		//GetWindowRect(hwndToolBar, &rcTB);
		//GetWindowRect(hwndStatusBar, &rcSB);
		//rect.top += rcTB.bottom - rcTB.top;
		//rect.bottom -= rcSB.bottom - rcSB.top;
		//for (auto& ball : Balls)
		//	ball.move(hDC, rect);
		SelectObject(hDC, hFontOriginal);
		DeleteObject(hFontTime);

		//
		EndPaint(hWnd, &ps);

		break;

	case WM_TIMER:
			GetClientRect(hWnd, &rect);
			InvalidateRect(hWnd, &rect, TRUE);
		break;

	case WM_SIZE:
		InvalidateRect(hWnd, &rect, TRUE);

		break;

		//case WM_CLOSE:
		//	userReply = MessageBox(hWnd, L"Закрыть приложение?",
		//		L"", MB_YESNO | MB_ICONQUESTION);
		//	if (IDYES == userReply)
		//		DestroyWindow(hWnd);
		//	break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}