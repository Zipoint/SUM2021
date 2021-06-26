/* FILE NAME   : u_hmap.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  mh5PRIM Land;
  VEC LandLoc;
  INT TexId;
} mh5UNIT_HMAP;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitInit( mh5UNIT_HMAP *Uni, mh5ANIM *Ani )
{
  INT w, h, i, j;
  mh5VERTEX *V;
  HBITMAP hBm;
  BITMAP bm;

  if ((hBm = LoadImage(NULL, "BIN/HEIGHTS/hmupp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if ((V = malloc(sizeof(mh5VERTEX) * w * h)) != NULL)
    {
      if (bm.bmBitsPixel == 8)
      {
        BYTE *Bits = bm.bmBits;

        for (i = 0; i < h; i++)
          for (j = 0; j < w; j++)
          {
            INT hgt = Bits[(h - 1 - i) * bm.bmWidthBytes + j * (bm.bmBitsPixel / 8)];
            V[i * w + j].P = VecSet(j / (w - 1.0),
                                    hgt / 255.0,
                                    1 - i / (h - 1.0));
          }
      }
      MH5_RndPrimGridEvalNormals(w, h, V);
      MH5_RndPrimCreateGrid(&Uni->Land, w, h, V);
      free(V);
    }
  }
  Uni->LandLoc = VecSet(-50, 0, -50);
} /* End of 'MH5_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitResponse( mh5UNIT_HMAP *Uni, mh5ANIM *Ani )
{
} /* End of 'MH5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitRender( mh5UNIT_HMAP *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimDraw(&Uni->Land, MatrMulMatr(MatrScale(VecSet(132, 7, 132)), MatrTranslate(Uni->LandLoc)));
} /* End of 'MH5_UnitRender' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitClose( mh5UNIT_HMAP *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimFree(&Uni->Land);
} /* End of 'MH5_UnitClose' function */

mh5UNIT * MH5_UnitCreateLand( VOID )
{
  mh5UNIT *Uni;

  if ((Uni = MH5_AnimUnitCreate(sizeof(mh5UNIT_HMAP))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MH5_UnitInit;
  Uni->Response = (VOID *)MH5_UnitResponse;
  Uni->Render = (VOID *)MH5_UnitRender;
  Uni->Close = (VOID *)MH5_UnitClose;

  return Uni;
} /* End of 'MH5_UnitCreateBall' function */

/* END OF 'u_hmap.c' FILE */