/* FILE NAME  : rndfnt.c
 * PROGRAMMER : MH5
 * LAST UPDATE: 24.06.2021
 * PURPOSE    : 3D animation project.
 *              Render subsystem implementation module.
 *              Font handle module.
 */

#include <stdio.h>
#include <string.h>

#include "rndres.h"

/***
 * Font handle
 ***/


/* Font description structure */
typedef struct tagmh5FONT
{
  DWORD LineH, BaseH; /* Font line height and base line height in pixels */
  FLT AdvanceX[256];  /* Every letter shift right value (0 if no letter present) */
} mh5FONT;

/* Current font description */

/* Font data */
static mh5FONT MH5_RndFntFont;

/* Font characters primitives */
static mh5PRIM MH5_RndFntChars[256];

/* Font material */
static INT MH5_RndFntMtlNo;


/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MH5_RndFntLoad( CHAR *FileName )
{
  FILE *F;
  INT i;
  DWORD Sign, W, H, *tex;
  mh5VERTEX Chars[256][4];
  mh5MATERIAL mtl;

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }
  fread(&MH5_RndFntFont, sizeof(mh5FONT), 1, F);
  fread(Chars, sizeof(mh5VERTEX) * 4, 256, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);
  if ((tex = malloc(W * H * 4)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(tex, 4, W * H, F);
  fclose(F);

  mtl = MH5_RndMtlGetDef();
  strncpy(mtl.Name, FileName, MH5_MAX_TEXTURE - 1);
  mtl.Tex[0] = MH5_RndTexAddImg(FileName, W, H, 4, tex);
  free(tex);
  mtl.ShdNo = MH5_RndShaderAdd("FONT");
  MH5_RndFntMtlNo = MH5_RndMtlAdd(&mtl);

  for (i = 0; i < 256; i++)
  {
    MH5_RndPrimFree(&MH5_RndFntChars[i]);
    MH5_RndPrimCreate(&MH5_RndFntChars[i], MH5_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    MH5_RndFntChars[i].MtlNo = MH5_RndFntMtlNo;
  }
  return TRUE;
} /* End of 'MH5_RndFntLoad' function */

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MH5_RndFntInit( VOID )
{
  MH5_RndFntLoad("BIN/FONTS/Book.g3df");
} /* End of 'MH5_RndFntInit' function */

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MH5_RndFntClose( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    MH5_RndPrimFree(&MH5_RndFntChars[i]);
  memset(MH5_RndFntChars, 0, sizeof(MH5_RndFntChars));
} /* End of 'MH5_RndFntInit' function */

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
VOID MH5_RndFntDraw( CHAR *Str, VEC Pos, FLT Size )
{
  VEC p = Pos;

  while (*Str != 0)
  {
    if (*Str == '\n')
      Pos.X = p.X, Pos.Y -= Size;
    else
      if (MH5_RndFntFont.AdvanceX[(UCHAR)*Str] != 0)
      {
        MH5_RndPrimDraw(&MH5_RndFntChars[(UCHAR)*Str],
          MatrMulMatr(MatrScale(VecSet(Size, Size, 1)), MatrTranslate(Pos)));
        Pos.X += MH5_RndFntFont.AdvanceX[(UCHAR)*Str] * Size;
      }
    Str++;
  }
} /* End of 'MH5_RndFntDraw' function */

/* END OF 'rndfnt.c' FILE */
