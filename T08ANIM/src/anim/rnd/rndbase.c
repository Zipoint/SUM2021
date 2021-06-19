/* FILE NAME   : rndbase.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing global module.
 */

#include "rnd.h"
#include <time.h>

VOID MH5_RndInit( HWND hWnd )
{
  HDC hDC;

  MH5_hWndRnd = hWnd;
  MH5_hBmRndFrame = NULL;
  hDC = GetDC(hWnd);
  MH5_hDCRndFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  MH5_RndCamSet(VecSet(0 ,10 ,10), VecSet(0 ,0, 0), VecSet(0, 1, 0));
}

VOID MH5_RndClose( VOID )
{
  if (MH5_hBmRndFrame != NULL)
    DeleteObject(MH5_hBmRndFrame);
  DeleteDC(MH5_hDCRndFrame);
}

VOID MH5_RndProjSet( VOID )
{
  DBL ratio_x, ratio_y;

  ratio_x = ratio_y = MH5_RndProjSize / 2;
  if (MH5_RndFrameW > MH5_RndFrameH)
    ratio_x *= (DBL)MH5_RndFrameW / MH5_RndFrameH;
  else
    ratio_y *= (DBL)MH5_RndFrameH / MH5_RndFrameW;

  MH5_RndMatrProj =
    MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y, MH5_RndProjDist, MH5_RndProjFarClip);
  MH5_RndMatrVP = MatrMulMatr(MH5_RndMatrView, MH5_RndMatrProj);
}

VOID MH5_RndResize(INT W, INT H)
{
  HDC hDC;

  if (MH5_hBmRndFrame != NULL)
    DeleteObject(MH5_hBmRndFrame);
  hDC = GetDC(MH5_hWndRnd);
  MH5_hBmRndFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(MH5_hWndRnd, hDC);
  SelectObject(MH5_hDCRndFrame, MH5_hBmRndFrame);

  MH5_RndFrameW = W;
  MH5_RndFrameH = H;
  MH5_RndProjSet();
}

VOID MH5_RndStart( VOID )
{
  SelectObject(MH5_hDCRndFrame, GetStockObject(GRAY_BRUSH));
  SelectObject(MH5_hDCRndFrame, GetStockObject(NULL_PEN));
  Rectangle(MH5_hDCRndFrame, 0, 0, MH5_RndFrameW + 1, MH5_RndFrameH + 1);
  SelectObject(MH5_hDCRndFrame, GetStockObject(DC_PEN));
  SetDCPenColor(MH5_hDCRndFrame, RGB(255, 255, 255));
}

VOID MH5_RndEnd( VOID )
{
}

VOID MH5_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, MH5_RndFrameW, MH5_RndFrameH, MH5_hDCRndFrame, 0, 0, SRCCOPY);
}

VOID MH5_RndCamSet( VEC Loc, VEC At, VEC Up1 )
{
  MH5_RndMatrView = MatrView(Loc, At, Up1);
  MH5_RndMatrVP = MatrMulMatr(MH5_RndMatrView, MH5_RndMatrProj);
}

/* END OF 'rndbase.c' FILE */
