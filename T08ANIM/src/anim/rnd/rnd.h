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

VOID MH5_RndCamSet( VEC Loc, VEC At, VEC Up1 );
VOID MH5_RndCopyFrame( HDC hDC );
VOID MH5_RndEnd( VOID );
VOID MH5_RndStart( VOID );
VOID MH5_RndResize(INT W, INT H);
VOID MH5_RndProjSet( VOID );
VOID MH5_RndInit( HWND hWnd );
VOID MH5_RndClose( VOID );
BOOL MH5_RndPrimCreate( mh5PRIM *Pr, INT NoofV, INT NoofI );
VOID MH5_RndPrimFree( mh5PRIM *Pr );
BOOL MH5_RndPrimCreateSphere( mh5PRIM *Pr,VEC C, DBL R, INT SplitW, INT SplitH );
VOID MH5_RndPrimDraw( mh5PRIM *Pr, MATR World );
BOOL MH5_RndPrimCreateGrid( mh5PRIM *Pr, INT SplitW, INT SplitH );
BOOL MH5_RndPrimCreateTor( mh5PRIM *Pr, VEC C, DBL R, DBL r, INT SplitW, INT SplitH );
BOOL MH5_RndPrimCreatePlosk( mh5PRIM *Pr, VEC C, VEC D, VEC L, INT SplitW, INT SplitH );
BOOL MH5_RndPrimLoad( mh5PRIM *Pr, CHAR *FileName );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */