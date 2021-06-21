/* FILE NAME   : u_ball.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  mh5PRIM Ball;
} mh5UNIT_BALL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitInit( mh5UNIT_BALL *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimCreateSphere(&Uni->Ball, VecSet(0, 0, 0), 2, 20, 20);
} /* End of 'MH5_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitResponse( mh5UNIT_BALL *Uni, mh5ANIM *Ani )
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
static VOID MH5_UnitRender( mh5UNIT_BALL *Uni, mh5ANIM *Ani )
{
  //MH5_RndCamSet(PointTransform(VecSet(8, 15 + 0 * 5 * sin(Ani->Time), 5), MatrRotateY(-Ani->Time * 27 * 0)), VecSet(0 ,0 ,0), VecSet(0 ,1 ,0));
  MH5_RndPrimDraw(&Uni->Ball, MatrMulMatr(MatrRotateY(Ani->Time), MatrTranslate(VecSet(5, 2 * fabs(0.8 * sin(Ani->Time * 5)), 3))));
} /* End of 'MH5_UnitRender' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitClose( mh5UNIT_BALL *Uni, mh5ANIM *Ani )
{
  DeleteObject(&Uni->Ball);
} /* End of 'MH5_UnitClose' function */

mh5UNIT * MH5_UnitCreateBall( VOID )
{
  mh5UNIT *Uni;

  if ((Uni = MH5_AnimUnitCreate(sizeof(mh5UNIT_BALL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MH5_UnitInit;
  Uni->Response = (VOID *)MH5_UnitResponse;
  Uni->Render = (VOID *)MH5_UnitRender;
  Uni->Close = (VOID *)MH5_UnitClose;

  return Uni;
} /* End of 'MH5_UnitCreateBall' function */

/* END OF 'u_ball.c' FILE */