/* FILE NAME   : main.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "../anim/rnd/rnd.h"
#include "../units/units.h"

/* Window class name */
#define MH5_WND_CLASS_NAME "My Summer class name"
INT MH5_MouseWheel;
extern MH5_MouseWheel;

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
  INT i;

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
    "T09ANIM",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  //MH5_AnimAddUnit(MH5_UnitCreateBall());
  MH5_AnimAddUnit(MH5_UnitCreatePlosk());
  MH5_AnimAddUnit(MH5_UnitCreatePig());
  MH5_AnimAddUnit(MH5_UnitCreateSky());
  for (i = 0; i < 70; i++)
  MH5_AnimAddUnit(MH5_UnitCreateGrass());
  MH5_AnimAddUnit(MH5_UnitCreateCtrl());
  MH5_AnimAddUnit(MH5_UnitCreateFanse());
  MH5_AnimAddUnit(MH5_UnitCreateHouse());
  //MH5_AnimAddUnit(MH5_UnitCreateHelecopter());
  //MH5_AnimAddUnit(MH5_UnitCreateLand());

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
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 47, 1, NULL);
    MH5_AnimInit(hWnd);
    return 0;

  case WM_SIZE:
    MH5_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_PAINT:
    MH5_AnimRender();
    MH5_AnimCopyFrame();
    return 0;

  case WM_DESTROY:
    MH5_RndClose();
    MH5_AnimClose();
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
      MH5_AnimFlipFullScreen();
    return 0;

  case WM_MOUSEWHEEL:
    MH5_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* end of 'MH5_MyWindowFunc' function*/

/* END OF 'main.c' FILE */