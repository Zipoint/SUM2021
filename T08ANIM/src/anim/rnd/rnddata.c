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

typedef struct tagmh5VERTEX
{
  VEC P;  /* Vertex position */
} mh5VERTEX;

typedef struct tagmh5PRIM
{
  mh5VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */

  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */

  MATR Trans;   /* Additional transformation matrix */
} mh5PRIM;

/* END OF 'rnddata.h' FILE */
