/* FILE NAME   : main.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "../anim/rnd/rnd.h"
#include <time.h>


/* Window class name */
#define MH5_WND_CLASS_NAME "My Summer class name"

/* Forward declaration */
LRESULT CALLBACK MH5_MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  SetDbgMemHooks();

  /* Fill window class structure */
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpszClassName = MH5_WND_CLASS_NAME;
  wc.lpfnWndProc = MH5_MyWindowFunc;


  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Window creation */
  hWnd = 
    CreateWindow(MH5_WND_CLASS_NAME,
    "T08ANIM",
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
} /* end of 'WinMain' function*/


/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *      HWND hWnd;
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK MH5_MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  DBL t = clock() / 1000.0;
  static mh5PRIM PrT, PrS, PrL, PrF, PrH, PrH2;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;

  case WM_SIZE:
    MH5_RndResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);

  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
      /*FlipFullScreen(hWnd); */
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 47, 1, NULL);
    MH5_RndInit(hWnd);
    MH5_RndPrimCreateTor(&PrT, VecSet(0, 0, 0), 8, 3, 40, 40);
    MH5_RndPrimCreateSphere(&PrS, VecSet(0, 0, 0), 2, 10, 10);
    MH5_RndPrimLoad(&PrL, "pig.obj");
    MH5_RndPrimLoad(&PrH, "girl.obj");
    MH5_RndPrimLoad(&PrH2, "cow.obj");
    MH5_RndPrimCreatePlosk(&PrF, VecSet(-8, 0 ,8), VecSet(18, 0 ,0), VecSet(0, 0, -18), 8, 8);
    return 0;

  case WM_TIMER:
    MH5_RndStart();
    SelectObject(MH5_hDCRndFrame, GetStockObject(DC_PEN));
    SetDCPenColor(MH5_hDCRndFrame, RGB(255, 255, 255));
    MH5_RndCamSet(PointTransform(VecSet(8, 15 + 0 * 5 * sin(t), 5), MatrRotateY(-t * 27)), VecSet(0 ,0 ,0), VecSet(0 ,1 ,0));

    //MH5_RndPrimDraw(&PrT, MatrMulMatr(MatrRotateY(30 * clock() / 1000.0), MatrTranslate(VecSet(0, 2 * fabs(0.8 * sin(clock() / 1000.0 * 5)), 0))));
    MH5_RndPrimDraw(&PrS, MatrMulMatr(MatrRotateY(30 * clock() / 1000.0), MatrTranslate(VecSet(5, 2 * fabs(0.8 * sin(clock() / 1000.0 * 5)), 3))));
    MH5_RndPrimDraw(&PrF, MatrIdentity());
    MH5_RndPrimDraw(&PrL, MatrMulMatr3(MatrScale(VecVec1(0.1)), MatrRotateY(sin(3 * clock() / 1000.0) * 8), MatrTranslate(VecSet(sin(3 * clock() / 1000.0) * 8, 0, -7))));
    MH5_RndPrimDraw(&PrH, MatrMulMatr3(MatrScale(VecVec1(0.03)), MatrTranslate(VecSet(0, 0, 0)), MatrRotateX(0)));
    MH5_RndPrimDraw(&PrH2, MatrMulMatr3(MatrScale(VecVec1(0.5)), MatrTranslate(VecSet(-3, 0, 5)), MatrRotateX(0)));
    MH5_RndEnd();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    MH5_RndCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    MH5_RndClose();
    MH5_RndPrimFree(&PrT);
    MH5_RndPrimFree(&PrS);
    MH5_RndPrimFree(&PrF);
    MH5_RndPrimFree(&PrL);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;

  case WM_ERASEBKGND:
    return 1;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* end of 'MH5_MyWindowFunc' function*/

/* END OF 'main.c' FILE */