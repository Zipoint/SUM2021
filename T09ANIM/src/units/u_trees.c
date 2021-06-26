/* FILE NAME   : u_trees.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  mh5PRIMS Trees;
  VEC TreeLoc;
  DBL I, I2;
} mh5UNIT_TREES;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitInit( mh5UNIT_TREES *Uni, mh5ANIM *Ani )
{
  Uni->I = Rnd1();
  Uni->I2 = Rnd1();
  Uni->TreeLoc = VecSet(0, 0, 0);
  MH5_RndPrimsLoad(&Uni->Trees, "BIN/MODELS/grass.g3dm");
} /* End of 'MH5_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitResponse( mh5UNIT_TREES *Uni, mh5ANIM *Ani )
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
static VOID MH5_UnitRender( mh5UNIT_TREES *Uni, mh5ANIM *Ani )
{
  Uni->TreeLoc = VecSet(Uni->I * 40, 0, Uni->I2 * 40);
  MH5_RndPrimsDraw(&Uni->Trees, MatrMulMatr(MatrScale(VecVec1(0.1)), MatrTranslate(Uni->TreeLoc)));
} /* End of 'MH5_UnitRender' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitClose( mh5UNIT_TREES *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimsFree(&Uni->Trees);
} /* End of 'MH5_UnitClose' function */

mh5UNIT * MH5_UnitCreateTrees( VOID )
{
  mh5UNIT *Uni;

  if ((Uni = MH5_AnimUnitCreate(sizeof(mh5UNIT_TREES))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MH5_UnitInit;
  Uni->Response = (VOID *)MH5_UnitResponse;
  Uni->Render = (VOID *)MH5_UnitRender;
  Uni->Close = (VOID *)MH5_UnitClose;

  return Uni;
} /* End of 'MH5_UnitCreateBall' function */

/* END OF 'u_trees.c' FILE */