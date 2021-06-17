/* FILE NAME   : rnd.h
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing declaration module.
 */

#ifndef __rnd_h_
#define __rnd_h_

#include "../../def.h"
extern HWND MH5_hWndRnd;
extern HDC MH5_hDCRndFrame;
extern HBITMAP MH5_hBmRndFrame;
extern INT MH5_RndFrameW, MH5_RndFrameH;

extern DBL
  MH5_RndProjSize,
  MH5_RndProjDist,
  MH5_RndProjFarClip;
extern MATR
  MH5_RndMatrView,
  MH5_RndMatrProj,
  MH5_RndMatrVP;
extern struct tagmh5VERTEX;
extern struct tagmh5PRIM;

VOID MH5_RndCamSet( VEC Loc, VEC At, VEC Up1 );
VOID MH5_RndCopyFrame( HDC hDC );
VOID MH5_RndEnd( VOID );
VOID MH5_RndStart( VOID );
VOID MH5_RndResize(INT W, INT H);
VOID MH5_RndProjSet( VOID );
VOID MH5_RndInit( HWND hWnd );
VOID MH5_RndClose( VOID );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */