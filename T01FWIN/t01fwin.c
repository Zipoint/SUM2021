#include <windows.h>

#define WND_CLASS_NAME "My Summer class name"

LRESULT CALLBACK MyWindowFunc( HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
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
    "Title",
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
  HPEN hPen, hPenOld;
  LOGBRUSH lb;
  INT x, y;
  static INT h, w;

  switch (Msg)
  {
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 47, 30, NULL);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_LBUTTONDOWN:
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    hDC = GetDC(hWnd);
    Ellipse(hDC, x + 100, y + 100, x - 100, y - 100);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    /*hPen = CreatePen(PS_SOLID, 5, RGB(255 ,0, 0));*/
    SetBkMode(hDC, OPAQUE);
    SetBkColor(hDC, RGB(0, 255, 110));

    lb.lbStyle = BS_HATCHED;
    lb.lbHatch = HS_BDIAGONAL;
    lb.lbColor = RGB(255, 0, 0);
    /*hPen = ExtCreatePen(PS_GEOMETRIC | PS_DASH, 8, &lb, 0, NULL);*/
    Ellipse(hDC, 10, 10, 900, 900);

    hPen = GetStockObject(BLACK_PEN);
    hPenOld = SelectObject(hDC, hPen);

    SelectObject(hDC, GetStockObject(DC_BRUSH));
    /*SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, RGB(0, 0, 0)); */
    SetDCBrushColor(hDC, RGB(255 ,255 ,0));
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    MoveToEx(hDC, w / 2, h / 2, NULL);
    LineTo(hDC, pt.x, pt.y);
    LineTo(hDC, 500, 400);

    Rectangle(hDC, 10, 10, 300, 300);
    Ellipse(hDC, 10, 10, 300, 300);

    SelectObject(hDC, hPenOld);
    DeleteObject(hPen);

    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}