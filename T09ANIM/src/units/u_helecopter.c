/* FILE NAME   : u_helecopter.c
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
} mh5UNIT_HELECOPTER;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitInit( mh5UNIT_HELECOPTER *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimsLoad(&Uni->prs, "BIN/MODELS/Hitler.g3dm");
  Uni->prs.Trans = MatrRotateX(0);
} /* End of 'MH5_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitResponse( mh5UNIT_HELECOPTER *Uni, mh5ANIM *Ani )
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
static VOID MH5_UnitRender( mh5UNIT_HELECOPTER *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimsDraw(&Uni->prs, MatrMulMatr(MatrScale(VecVec1(0.1)), MatrTranslate(VecSet(0, 10, 0))));
} /* End of 'MH5_UnitRender' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitClose( mh5UNIT_HELECOPTER *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimsFree(&Uni->prs);
} /* End of 'MH5_UnitClose' function */

mh5UNIT * MH5_UnitCreateHelecopter( VOID )
{
  mh5UNIT *Uni;

  if ((Uni = MH5_AnimUnitCreate(sizeof(mh5UNIT_HELECOPTER))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MH5_UnitInit;
  Uni->Response = (VOID *)MH5_UnitResponse;
  Uni->Render = (VOID *)MH5_UnitRender;
  Uni->Close = (VOID *)MH5_UnitClose;

  return Uni;
} /* End of 'MH5_UnitCreateBall' function */

/* END OF 'u_helecopter.c' FILE */