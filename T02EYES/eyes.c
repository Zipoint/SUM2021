/* FILE NAME: EYES.C
 * PROGRAMMER: MH5
 * DATE 10.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <math.h>

#include <windows.h>

#define WND_CLASS_NAME "My Summer class name"

LRESULT CALLBACK MyWindowFunc( HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawEye( HDC hDC, INT x, INT y, INT x1, INT y1, INT Mx, INT My);

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
    "T02EYES",
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
  INT i;
  static INT h, w;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 47, 10, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, TRUE);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, w, h);
    srand(30);
    for (i = 0; i < 101; i++)
      DrawEye(hMemDC, rand() % w, rand() % h, 50 + rand() % 47, 18 + rand() &8, pt.x, pt.y);
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
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID DrawEye( HDC hMemDC, INT x, INT y, INT x1, INT y1, INT Mx, INT My )
{
  INT len, R = 100, R1 = 40;
  HPEN hPenOld;

  hPenOld = SelectObject(hMemDC, GetStockObject(BLACK_PEN));

  len = (INT)(sqrt((My - y) * (My - y) + (Mx - x) * (Mx - x)));
  if (len == 0)
    len++;
  y1 = y + (My - y) * (R - R1) / len;
  x1 = x + (Mx - x) * (R - R1) / len;

  SelectObject(hMemDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hMemDC, RGB(200, 200, 200));
  Ellipse(hMemDC, x - R, y - R, x + R, y + R);
  SetDCBrushColor(hMemDC, RGB(0, 0, 0));
  if (len > R - R1)
    Ellipse(hMemDC, x1 - R1, y1 - R1, x1 + R1, y1 + R1);
  else
    Ellipse(hMemDC, Mx - R1, My - R1, Mx + R1, My + R1);

  SelectObject(hMemDC, hPenOld);
}