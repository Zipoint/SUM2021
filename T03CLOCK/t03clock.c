/* FILE NAME: CLOCK.C
 * PROGRAMMER: MH5
 * DATE 11.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#pragma warning(disable: 4244)

#include <stdio.h>
#include <math.h>

#include <windows.h>

#define WND_CLASS_NAME "My Summer class name"

LRESULT CALLBACK MyWindowFunc( HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawHand( HDC hDC, INT X0, INT Y0, DOUBLE AngleInDegree, INT L, INT W );
VOID FlipFullScreen( HWND hWnd );

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
    "T03CLOCK",
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
  BITMAP bm, bmLogo;
  HPEN hPenOld;
  SYSTEMTIME st;
  HFONT hFnt, hFntOld;
  DOUBLE pi = 3.14159265358979323846, angleS, angleM, angleH;
  INT size;
  CHAR Buf[102];
  COLORREF cl;
  static CHAR *WD[] =
  {
    "бя", "ом", "бр", "яп", "вр", "ор", "яа"
  };
  static COLORREF Back;
  static INT h, w;
  static HDC hDCLogo, hMemDC, hMemDCLogo;
  static HBITMAP hBmLogo, hBm, hBmLogoXor, hBmLogoAND;

  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBmLogo != NULL)
      DeleteObject(hBmLogo);
    hDC = GetDC(hWnd);
    if (hBmLogoXor != NULL)
      DeleteObject(hBmLogoXor);
    if (hBmLogoAND != NULL)
      DeleteObject(hBmLogoAND);
    hBmLogo = LoadImage(NULL, "12.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLogoXor = LoadImage(NULL, "XOR.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLogoAND = LoadImage(NULL, "OR.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hDCLogo, hBmLogo);
    Back = GetPixel(hDCLogo, 30, 30);
    SelectObject(hMemDC, hBm);
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
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, Back);
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    size = w < h ? w : h;

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    GetObject(hBmLogo, sizeof(bm), &bm);
    GetObject(hBmLogoAND, sizeof(bmLogo), &bmLogo);
    StretchBlt(hMemDC, (w - size) / 2, (h - size) / 2, size, size, hDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    GetLocalTime(&st);
    hPenOld = SelectObject(hMemDC, CreatePen(PS_SOLID, 5, RGB(0 ,0, 0)));

    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    angleS = (st.wSecond + st.wMilliseconds / 1000.0) * 2 * pi / 60;
    LineTo(hMemDC, w / 2 + size * (sin(angleS) / 3), h / 2 - size * (cos(angleS) / 3));
    //DrawHand(hMemDC, w / 2, h / 2, angleS, size / 20, size / 18);

    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    angleM = (st.wMinute + st.wSecond / 60.0) * 2 * pi / 60;
    LineTo(hMemDC, w / 2 + size * (sin(angleM) / 4), h / 2 - size * (cos(angleM) / 4));

    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    angleH = (st.wHour + st.wSecond / 3600.0 + st.wMinute / 60.0) * 2 * pi / 12;
    LineTo(hMemDC, w / 2 + size * (sin(angleH) / 5), h / 2 - size * (cos(angleH) / 5));

    hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_SWISS, "");
    hFntOld = SelectObject(hMemDC, hFnt);
    SelectObject(hMemDC, hFntOld);
    SetTextColor(hMemDC, RGB(0, 0, 0));
    SetBkMode(hMemDC, TRANSPARENT);
    cl = GetPixel(hMemDC, (w - bm.bmWidth) / 2 + 1, (h - bm.bmHeight) / 2 + 1);
    TextOut(hMemDC, w / 2 - 45, (h - size) / 2 + size / 6, Buf, sprintf(Buf, "DayOfWeek:%s", WD[st.wDayOfWeek]));
    TextOut(hMemDC, w / 2 - 45, (h - size) / 2 + size / 5, Buf, sprintf(Buf, "Date:%i.%i.%i", st.wDay, st.wMonth, st.wYear));
    TextOut(hMemDC, w / 2 - 45, (h - size) / 2 + size / 4, Buf, sprintf(Buf, "Time:%i:%i:%i", st.wSecond, st.wMinute, st.wHour));

    SelectObject(hMemDCLogo, hBmLogoAND);
    BitBlt(hMemDC, pt.x - bmLogo.bmWidth / 2, pt.y - bmLogo.bmHeight / 2, bmLogo.bmWidth, bmLogo.bmHeight, hMemDCLogo, 0, 0, SRCAND);
    SelectObject(hMemDCLogo, hBmLogoXor);
    BitBlt(hMemDC, pt.x - bmLogo.bmWidth / 2, pt.y - bmLogo.bmHeight / 2, bmLogo.bmWidth, bmLogo.bmHeight, hMemDCLogo, 0, 0, SRCINVERT);

    SelectObject(hMemDC, hPenOld);

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
    DeleteDC(hMemDCLogo);
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID DrawHand( HDC hDC, INT X0, INT Y0, DOUBLE AngleInRad, INT L, INT W )
{
  POINT pnts[] =
  {
    {X0, L + Y0}, {X0 + W, Y0}, {X0, Y0 - W}, {X0 - W, Y0}
  };
  POINT pnts1[sizeof(pnts) / sizeof(pnts[0])];
  INT i;
  DOUBLE si, co;
  co = cos(AngleInRad);
  si = sin(AngleInRad);

  for (i = 0; i < sizeof(pnts) / sizeof(pnts[0]); i++)
  {
    pnts1[i].x = X0 + pnts[i].x * co + pnts[i].y * si;
    pnts1[i].y = Y0 + pnts[i].y * co - pnts[i].x * si;
  }
  Polygon(hDC, pnts1, sizeof(pnts) / sizeof(pnts[0]));
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