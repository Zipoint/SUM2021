/* FILE NAME   : rnd.h
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing declaration module.
 */

#ifndef __rnd_h_
#define __rnd_h_

#include "../../def.h"
#include <stdio.h>

#define GLEW_STATIC
#include <glew.h>

extern HWND MH5_hRndWnd;
extern HDC MH5_hRndDC;
extern INT MH5_RndFrameW, MH5_RndFrameH;
extern HGLRC MH5_hRndGLRC;

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
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
} mh5VERTEX;

typedef struct tagmh5PRIM
{
  mh5VERTEX *V;
  INT VBuf, VA, IBuf, NumOfElements, NumOfV;

  MATR Trans;   /* Additional transformation matrix */
} mh5PRIM;

VOID MH5_RndCamSet( VEC Loc, VEC At, VEC Up1 );
VOID MH5_RndCopyFrame( VOID );
VOID MH5_RndEnd( VOID );
VOID MH5_RndStart( VOID );
VOID MH5_RndResize(INT W, INT H);
VOID MH5_RndProjSet( VOID );
VOID MH5_RndInit( HWND hWnd );
VOID MH5_RndClose( VOID );
VOID MH5_RndPrimCreate( mh5PRIM *Pr, mh5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
VOID MH5_RndPrimFree( mh5PRIM *Pr );
BOOL MH5_RndPrimCreateSphere( mh5PRIM *Pr,VEC C, DBL R, INT SplitW, INT SplitH );
VOID MH5_RndPrimDraw( mh5PRIM *Pr, MATR World );
BOOL MH5_RndPrimCreateGrid( mh5PRIM *Pr, INT SplitW, INT SplitH, mh5VERTEX *V );
BOOL MH5_RndPrimCreateTor( mh5PRIM *Pr, VEC C, DBL R, DBL r, INT SplitW, INT SplitH );
BOOL MH5_RndPrimCreatePlosk( mh5PRIM *Pr, VEC C, DBL D, INT SplitW, INT SplitH );
BOOL MH5_RndPrimLoad( mh5PRIM *Pr, CHAR *FileName );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */