/* FILE NAME   : rnd.h
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing declaration module.
 */

#ifndef __rnd_h_
#define __rnd_h_

#include "../../def.h"
#include "../anim.h"
#include "res/rndres.h"
#include <stdio.h>

#define GLEW_STATIC
#include <glew.h>

extern HWND MH5_hRndWnd;
extern HDC MH5_hRndDC;
extern INT MH5_RndFrameW, MH5_RndFrameH;
extern HGLRC MH5_hRndGLRC;
extern VEC MH5_RndCamLoc;

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

typedef enum tagmh5PRIM_TYPE
{
  MH5_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles */
  MH5_RND_PRIM_TRISTRIP, /* Triangle strip - array of stripped triangles */
  MH5_RND_PRIM_LINES,    /* Line segments (by 2 points) */
  MH5_RND_PRIM_POINTS,   /* Arrauy of points */
} mh5PRIM_TYPE;

typedef struct tagmh5PRIM
{
  mh5PRIM_TYPE Type;
  mh5VERTEX *V;
  INT VBuf, VA, IBuf, NumOfElements, NumOfV;

  INT MtlNo;
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
VOID MH5_RndPrimCreate( mh5PRIM *Pr, mh5PRIM_TYPE Type, mh5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
VOID MH5_RndPrimFree( mh5PRIM *Pr );
BOOL MH5_RndPrimCreateSphere( mh5PRIM *Pr,VEC C, DBL R, INT SplitW, INT SplitH );
VOID MH5_RndPrimDraw( mh5PRIM *Pr, MATR World );
BOOL MH5_RndPrimCreateGrid( mh5PRIM *Pr, INT SplitW, INT SplitH, mh5VERTEX *V );
BOOL MH5_RndPrimCreateTor( mh5PRIM *Pr, VEC C, DBL R, DBL r, INT SplitW, INT SplitH );
BOOL MH5_RndPrimCreatePlosk( mh5PRIM *Pr, VEC C, DBL D, INT SplitW, INT SplitH );
BOOL MH5_RndPrimLoad( mh5PRIM *Pr, CHAR *FileName );
INT MH5_RndShaderAdd( CHAR *FileNamePrefix );

VOID MH5_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text );

/* Load shader text from file function.
 * ARGUMENTS:
 *   - text file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text.
 */
CHAR * MH5_RndLoadTextFromFile( CHAR *FileName );

/* Load shader program function.
 * ARGUMENTS:
 *   - shader folder prefix (in 'BIN/SHADERS/***'):
 *       CHAR *FileNamePrefix;
 * RETUNS:
 *   (INT) load shader program Id.
 */
INT MH5_RndShdLoad( CHAR *FileNamePrefix );

/* Delete shader program function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETUNS: None.
 */
VOID MH5_RndShdFree( INT ProgId );

/* Shader stock initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MH5_RndShadersInit( VOID );

/* Shader stock deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MH5_RndShadersClose( VOID );

/* Shader add to stock function.
 * ARGUMENTS:
 *   - shader folder prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader number in stock.
 */
INT MH5_RndShaderAdd( CHAR *FileNamePrefix );

/* Shader stock update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MH5_RndShadersUpdate( VOID );
VOID MH5_RndPrimGridEvalNormals( INT SplitW, INT SplitH, mh5VERTEX *V );

/***
 * Primitive collection support
 ***/

/* Primitive collection data type */
typedef struct tagmh5PRIMS
{
  INT NumOfPrims; /* Number of primitives in array */  
  mh5PRIM *Prims; /* Array of primitives */
  MATR Trans;     /* Common transformation matrix */
} mh5PRIMS;

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       mh5PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL MH5_RndPrimsCreate( mh5PRIMS *Prs, INT NumOfPrims );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       mh5PRIMS *Prs;
 * RETURNS: None.
 */
VOID MH5_RndPrimsFree( mh5PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       mh5PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID MH5_RndPrimsDraw( mh5PRIMS *Prs, MATR World );

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       mh5PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL MH5_RndPrimsLoad( mh5PRIMS *Prs, CHAR *FileName );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */