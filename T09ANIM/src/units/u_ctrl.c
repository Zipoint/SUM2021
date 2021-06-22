/* FILE NAME   : u_ctrl.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  DBL AngleSpeed, Speed;
  VEC CamLoc, CamDir, CamRight, CamUp;
} mh5UNIT_CTRL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitInit( mh5UNIT_CTRL *Uni, mh5ANIM *Ani )
{
  Uni->CamLoc = VecSet(0, 16, 16);
  Uni->AngleSpeed = 40;
  Uni->Speed = 40;
} /* End of 'MH5_UnitInit' function */


/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitResponse( mh5UNIT_CTRL *Uni, mh5ANIM *Ani )
{
  Uni->CamDir = VecSet(-MH5_RndMatrView.A[0][2], -MH5_RndMatrView.A[1][2], -MH5_RndMatrView.A[2][2]);
  Uni->CamRight = VecSet(MH5_RndMatrView.A[0][0], MH5_RndMatrView.A[1][0], MH5_RndMatrView.A[2][0]);
  Uni->CamUp = VecSet(MH5_RndMatrView.A[0][1], MH5_RndMatrView.A[1][1], MH5_RndMatrView.A[2][1]);
  /* Mouse control */
  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(-Ani->Keys[VK_LBUTTON] *
        Ani->GlobalDeltaTime * Uni->AngleSpeed * Ani->Mdx));
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed * Ani->Mdz));
  Uni->CamLoc =
     PointTransform(Uni->CamLoc,
       MatrRotate(-Ani->Keys[VK_LBUTTON] *
       Ani->GlobalDeltaTime * Uni->AngleSpeed * Ani->Mdy, Uni->CamRight));

  /* JOY control */
  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->GlobalDeltaTime * Uni->AngleSpeed * Ani->JX));
  Uni->CamLoc =
     PointTransform(Uni->CamLoc,
       MatrRotate(Ani->GlobalDeltaTime * Uni->AngleSpeed * Ani->JY, Uni->CamRight));
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed * Ani->JBut[7] / 2));
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
     VecMulNum(Uni->CamDir, -Ani->GlobalDeltaTime * Uni->Speed * Ani->JBut[6] / 2));
  if (Ani->JButClick[0] == 1)
    MH5_AnimFlipFullScreen();
  if (Ani->JBut[4] == 1)
    SendMessage(MH5_hRndWnd, WM_CLOSE, 0, 0);
} /* End of 'MH5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitRender( mh5UNIT_CTRL *Uni, mh5ANIM *Ani )
{
  MH5_RndCamSet(Uni->CamLoc, Uni->CamDir, VecSet(0, 1, 0));
} /* End of 'MH5_UnitRender' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitClose( mh5UNIT_CTRL *Uni, mh5ANIM *Ani )
{
} /* End of 'MH5_UnitClose' function */

mh5UNIT * MH5_UnitCreateCtrl( VOID )
{
  mh5UNIT *Uni;

  if ((Uni = MH5_AnimUnitCreate(sizeof(mh5UNIT_CTRL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MH5_UnitInit;
  Uni->Response = (VOID *)MH5_UnitResponse;
  Uni->Render = (VOID *)MH5_UnitRender;
  Uni->Close = (VOID *)MH5_UnitClose;

  return Uni;
} /* End of 'MH5_UnitCreateBall' function */

/* END OF 'u_ctrl.c' FILE */