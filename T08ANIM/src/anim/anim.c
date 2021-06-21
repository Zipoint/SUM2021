/* FILE NAME   : anim.h
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "anim.h"
mh5ANIM MH5_Anim;

VOID MH5_AnimAddUnit( mh5UNIT *Uni )
{
  if (MH5_Anim.NumOfUnits < MH5_MAX_UNITS)
    MH5_Anim.Units[MH5_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &MH5_Anim);
}

VOID MH5_AnimInit( HWND hWnd )
{
  MH5_TimerInit();
  MH5_AnimInputInit();

  MH5_Anim.hWnd = hWnd;
  MH5_RndInit(hWnd);
  MH5_Anim.hDC = MH5_hDCRndFrame;
}


VOID MH5_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < MH5_Anim.NumOfUnits; i++)
  {
    MH5_Anim.Units[i]->Close(MH5_Anim.Units[i], &MH5_Anim);
    free(MH5_Anim.Units[i]);
    MH5_Anim.Units[i] = NULL;
  }

  MH5_Anim.NumOfUnits = 0;
  MH5_RndClose();
}

VOID MH5_AnimResize( INT W, INT H )
{
  MH5_Anim.W = W;
  MH5_Anim.H = H;

  MH5_RndResize(W, H);
}

VOID MH5_AnimCopyFrame( HDC hDC )
{
  MH5_RndCopyFrame(hDC);
}

VOID MH5_AnimRender( VOID )
{
  INT i;
  static CHAR Buf[100];

  MH5_TimerResponse();
  MH5_AnimInputResponse();

  for (i = 0; i < MH5_Anim.NumOfUnits; i++)
    MH5_Anim.Units[i]->Response(MH5_Anim.Units[i], &MH5_Anim);

  MH5_RndStart();
  for (i = 0; i < MH5_Anim.NumOfUnits; i++)
    MH5_Anim.Units[i]->Render(MH5_Anim.Units[i], &MH5_Anim);

  SetBkMode(MH5_Anim.hDC, TRANSPARENT);
  SetTextColor(MH5_Anim.hDC, RGB(0, 255, 0));
  TextOut(MH5_Anim.hDC, 8, 8, Buf, sprintf(Buf, "FPS: %.3f", MH5_Anim.FPS));
  MH5_RndEnd();
}

VOID MH5_AnimFlipFullScreen( VOID )
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
    GetWindowRect(MH5_Anim.hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(MH5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmon, &moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(MH5_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(MH5_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    IsFullScreen = FALSE;

    /* Restore window size and position */
    SetWindowPos(MH5_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
} /* End of 'FlipFullScreen' function */

/* END OF 'anim.h' FILE */