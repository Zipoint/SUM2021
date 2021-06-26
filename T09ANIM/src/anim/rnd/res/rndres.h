/* FILE NAME   : rndres.h
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing declaration module.
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "../rnd.h"

#define MH5_MAX_SHADERS 30
#define MH5_MAX_MATERIALS 300
#define MH5_MAX_TEXTURE 300
#define MH5_STR_MAX 300

typedef struct tagmh5SHADER
{
  CHAR Name[MH5_STR_MAX];
  INT ProgId;
} mh5SHADER;

extern mh5SHADER MH5_RndShaders[MH5_MAX_SHADERS];
extern INT MH5_RndShadersSize;

typedef struct tagmh5TEXTURE
{
  CHAR Name[MH5_STR_MAX];
  INT W, H;
  UINT TexId;
} mh5TEXTURE;

extern mh5TEXTURE MH5_RndTextures[MH5_MAX_TEXTURE];
extern INT MH5_RndTextureSize;

typedef struct tagmh5MATERIAL
{
  CHAR Name[MH5_STR_MAX];

  VEC Ka, Kd, Ks;
  FLT Ph;

  FLT Trans;
  INT Tex[8];

  INT ShdNo;
  CHAR ShaderStr[300];
} mh5MATERIAL;

extern mh5MATERIAL MH5_RndMaterials[MH5_MAX_MATERIALS];
extern INT MH5_RndMaterialsSize;

INT MH5_RndMtlAdd( mh5MATERIAL *Mtl );
INT MH5_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
INT MH5_RndTexAddFromFile( CHAR *FileName);
VOID MH5_RndTexInit( VOID );
VOID MH5_RndTexClose( VOID );
VOID MH5_RndMtlClose( VOID );
VOID MH5_RndMtlInit( VOID );

/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MH5_RndFntLoad( CHAR *FileName );

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MH5_RndFntInit( VOID );

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MH5_RndFntClose( VOID );

/* Draw string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC Pos;
 *   = font size:
 *      FLT Size;
 * RETURNS: None.
 */
VOID MH5_RndFntDraw( CHAR *Str, VEC Pos, FLT Size );
mh5MATERIAL MH5_RndMtlGetDef( VOID );
VOID MH5_RndResClose( VOID );
VOID MH5_RndResInit( VOID );
INT MH5_RndMtlApply( INT MtlNo );


#endif /* __rndres_h_ */

/* END OF 'rndres.h' FILE */