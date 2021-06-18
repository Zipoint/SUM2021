/* FILE NAME   : rnddata.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing  module.
 */

#include "rnd.h"

HWND MH5_hWndRnd;
HDC MH5_hDCRndFrame;
HBITMAP MH5_hBmRndFrame;
INT MH5_RndFrameW, MH5_RndFrameH;

DBL
  MH5_RndProjSize = 0.1,
  MH5_RndProjDist = 0.1,
  MH5_RndProjFarClip = 300;
MATR
  MH5_RndMatrView,
  MH5_RndMatrProj,
  MH5_RndMatrVP;

/* END OF 'rnddata.h' FILE */
