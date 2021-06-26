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
  VEC CamDir, CamRight, CamUp, At;
  FLT RotY, RotX, CamDist;
  BOOL InWire, InJOY;
} mh5UNIT_CTRL;
mh5UNIT_CTRL MH5_CTRL;

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
  Uni->Speed = 20;
  Uni->At = VecSet(-30, 30, 1);
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

  Uni->CamDist = 0.1;
  if (!Uni->InJOY)
  {
    Uni->RotY += Ani->GlobalDeltaTime * (1.5 * Ani->JX);
    Uni->RotX += Ani->GlobalDeltaTime * (1.5 * Ani->JY);
    Uni->At = VecAddVec(Uni->At, VecMulNum(Uni->CamDir, (Ani->JBut[7] - Ani->JBut[6]) * 10 * Ani->GlobalDeltaTime));
  }
  else
  {
    Uni->RotY += Ani->GlobalDeltaTime * (-0.2 * Ani->Keys[VK_LBUTTON] * Ani->Mdx + 3 * 0.47 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
    Uni->RotX += Ani->GlobalDeltaTime * (-0.2 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 0.47 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));
    Uni->RotY += Ani->GlobalDeltaTime * (1.5 * -(Ani->Keys['A'] - Ani->Keys['D']));
    Uni->RotX += Ani->GlobalDeltaTime * (1.5 * -(Ani->Keys['W'] - Ani->Keys['S']));
    Uni->At = VecAddVec(Uni->At, VecMulNum(Uni->CamDir, Ani->Mdz * 0.3 * Ani->GlobalDeltaTime));
    Uni->At = VecAddVec(Uni->At, VecMulNum(Uni->CamDir, Ani->Mdz * Ani->Keys[VK_SHIFT] * 5 * Ani->GlobalDeltaTime));
    Uni->At = VecAddVec(Uni->At, VecMulNum(Uni->CamDir, Ani->Mdz * Ani->Keys[VK_SHIFT] * Ani->Keys[VK_CONTROL] * 50 * Ani->GlobalDeltaTime));
  }

  if (Ani->JButClick[0] == 1)
    MH5_AnimFlipFullScreen();
  if (Ani->JBut[4] == 1)
    SendMessage(MH5_hRndWnd, WM_CLOSE, 0, 0);
  if (Ani->JButClick[1] == 1)
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick[VK_NUMPAD1])
    Uni->InJOY = !Uni->InJOY;
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
  static CHAR buf[100];
  MATR m1, m2, m3;

  m1 = MH5_RndMatrView;
  m2 = MH5_RndMatrProj;
  m3 = MH5_RndMatrVP;

  MH5_RndMatrView = MatrIdentity();
  MH5_RndMatrProj = MatrOrtho(0, Ani->W, 0, Ani->H, -1, 1);
  MH5_RndMatrVP = MatrMulMatr(MH5_RndMatrView, MH5_RndMatrProj);

  sprintf(buf, "FPS: %.5f", Ani->FPS);

  MH5_RndFntDraw(buf, VecSet(0, Ani->H, 0), 30);

  MH5_RndMatrView = m1;
  MH5_RndMatrProj = m2;
  MH5_RndMatrVP = m3;

  MH5_RndCamSet(PointTransform(VecSet(0, 0, Uni->CamDist),
                             MatrMulMatr3(MatrRotateX(-18 * 5 / 2.0 * Uni->RotX),
                                          MatrRotateY(-102 * 5 / 8.0 * Uni->RotY),
                                          MatrTranslate(Uni->At))), Uni->At, VecSet(0, 1, 0));

  MH5_RndCamLoc = PointTransform(VecSet(0, 0, Uni->CamDist),
                             MatrMulMatr3(MatrRotateX(-18 * 5 / 2.0 * Uni->RotX),
                                          MatrRotateY(-102 * 5 / 8.0 * Uni->RotY),
                                          MatrTranslate(Uni->At))), Uni->At, VecSet(0, 1, 0);

  if (Ani->KeysClick[VK_NUMPAD0])
  {
    Uni->InWire = !Uni->InWire;
    if (Uni->InWire)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
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