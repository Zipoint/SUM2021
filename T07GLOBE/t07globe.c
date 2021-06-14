/* FILE NAME: GLOBE.C
 * PROGRAMMER: MH5
 * DATE 14.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#pragma warning(disable: 4244)

#include <stdio.h>
#include <math.h>
#include "globe.h"

#include <windows.h>

#define WND_CLASS_NAME "My Summer class name"

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  hWnd = 
    CreateWindow(WND_CLASS_NAME,
    "T07GLOBE",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);


  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hDC;
  POINT pt;
  SYSTEMTIME st;
  static COLORREF Back;
  static INT h, w;
  static HDC hDCLogo, hMemDC;
  static HBITMAP hBmLogo, hBm;

  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    GlbeSet(w / 2, h / 2, (w < h ? w : h) * 0.8);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
      FlipFullScreen(hWnd);
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 47, 10, NULL);
    hDC = GetDC(hWnd);
    hDCLogo = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    GetLocalTime(&st);
    GlobeDraw(hMemDC);

    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY:
    KillTimer(hWnd, 47);
    if (hBmLogo != NULL)
      DeleteObject(hBmLogo);
    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteDC(hMemDC);
    DeleteDC(hDCLogo);
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO moninfo;
    RECT rc;

    IsFullScreen = TRUE;

    /* Goto fullscreen mode */

    /* Store current window size and position */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmon, &moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    IsFullScreen = FALSE;

    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
} /* End of 'FlipFullScreen' function */