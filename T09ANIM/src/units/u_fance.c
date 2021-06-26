/* FILE NAME   : u_fance.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  mh5PRIMS prs;
} mh5UNIT_FANCE;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitInit( mh5UNIT_FANCE *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimsLoad(&Uni->prs, "BIN/MODELS/fancewood.g3dm");
} /* End of 'MH5_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitResponse( mh5UNIT_FANCE *Uni, mh5ANIM *Ani )
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
static VOID MH5_UnitRender( mh5UNIT_FANCE *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr(MatrTranslate(VecSet(0, 0, 2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr(MatrTranslate(VecSet(1.18, 0, 2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr(MatrTranslate(VecSet(-1.18, 0, 2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr(MatrTranslate(VecSet(0, 0, -2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr(MatrTranslate(VecSet(1.18, 0, -2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr(MatrTranslate(VecSet(-1.18, 0, -2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr3(MatrRotateY(90), MatrTranslate(VecSet(2, 0, 0.2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr3(MatrRotateY(90), MatrTranslate(VecSet(2, 0, 1.38)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr3(MatrRotateY(90), MatrTranslate(VecSet(2, 0, -1.18 + 0.2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr3(MatrRotateY(90), MatrTranslate(VecSet(-2, 0, 0.2)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr3(MatrRotateY(90), MatrTranslate(VecSet(-2, 0, 1.38)), MatrScale(VecSet(10, 2.5, 10))));
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr3(MatrRotateY(90), MatrTranslate(VecSet(-2, 0, -1.18 + 0.2)), MatrScale(VecSet(10, 2.5, 10))));
} /* End of 'MH5_UnitRender' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitClose( mh5UNIT_FANCE *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimsFree(&Uni->prs);
} /* End of 'MH5_UnitClose' function */

mh5UNIT * MH5_UnitCreateFanse( VOID )
{
  mh5UNIT *Uni;

  if ((Uni = MH5_AnimUnitCreate(sizeof(mh5UNIT_FANCE))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MH5_UnitInit;
  Uni->Response = (VOID *)MH5_UnitResponse;
  Uni->Render = (VOID *)MH5_UnitRender;
  Uni->Close = (VOID *)MH5_UnitClose;

  return Uni;
} /* End of 'MH5_UnitCreateBall' function */

/* END OF 'u_fance.c' FILE */