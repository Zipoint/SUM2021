/* FILE NAME: GLOBE.C
 * PROGRAMMER: MH5
 * DATE 14.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#pragma warning(disable: 4244)

#include "globe.h"

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
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
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

  /* Message loop */
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 30, 0);

  return 30;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  static HFONT hFnt, hFntOld;
  BITMAP bmLogo;
  PAINTSTRUCT ps;
  INT size;
  static INT h, w;
  static HDC hMemDC, hMemDCLogo;
  static HBITMAP hBm, hBmLogoXor, hBmLogoAND;
  static CHAR Buf[100];

  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    hBmLogoXor = LoadImage(NULL, "XOR.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLogoAND = LoadImage(NULL, "OR.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    GlobeSet(w, h, 1);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (wParam == 'P')
      GLB_IsPause = !GLB_IsPause;
    return 0;

  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
      FlipFullScreen(hWnd);
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 47, 1, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;

    GLB_TimerInit();
    return 0;

  case WM_TIMER:
    GLB_TimerResponse();
    GetObject(hBmLogoAND, sizeof(bmLogo), &bmLogo);
    SelectObject(hMemDC, GetStockObject(GRAY_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));
    SelectObject(hMemDC, GetStockObject(BLACK_PEN));
    GlobeDraw(hMemDC);

    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(0, 255, 0));
    TextOut(hMemDC, 8, 8, Buf, sprintf(Buf, "FPS: %.3f", GLB_FPS));
    size = w < h ? w : h;

    if (GLB_IsPause)
    {
      RECT rc = {0, 0, w, h};

      /*hFnt = CreateFont((w < h ? w : h) / 3, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_SWISS, "Consolas");
      SetTextColor(hMemDC, RGB(255, 0, 0));
      hFntOld = SelectObject(hMemDC, hFnt);
      DrawText(hMemDC, "PAUSE", 5, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
      SelectObject(hMemDC, hFntOld);*/
      SelectObject(hMemDCLogo, hBmLogoAND);
      StretchBlt(hMemDC, (w - size) / 2, (h - size) / 2, size, size, hMemDCLogo, 0, 0, bmLogo.bmWidth, bmLogo.bmHeight, SRCAND);
      SelectObject(hMemDCLogo, hBmLogoXor);
      StretchBlt(hMemDC, (w - size) / 2, (h - size) / 2, size, size, hMemDCLogo, 0, 0, bmLogo.bmWidth, bmLogo.bmHeight, SRCINVERT);
    }

    InvalidateRect(hWnd, NULL, FALSE);
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
    if (hBm != NULL)
      DeleteObject(hBm);
    if (hBmLogoAND != NULL)
      DeleteObject(hBmLogoAND);
    if (hBmLogoXor != NULL)
      DeleteObject(hBmLogoXor);
    DeleteObject(hFnt);
    DeleteObject(hFntOld);
    DeleteDC(hMemDC);
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

/* END OF 't07globe.c' FILE */