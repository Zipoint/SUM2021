/* FILE NAME   : u_pig.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  mh5PRIM Pig;
  VEC PigLoc, PigDir, PigRight, PigLocOld;
  VEC CamLoc, CamDir;
  FLT P;
} mh5UNIT_PIG;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitInit( mh5UNIT_PIG *Uni, mh5ANIM *Ani )
{
  Uni->PigDir = VecSet(1, 0, 0);
  Uni->PigLoc = VecSet(0, 0, 0);
  Uni->PigRight = VecSet(0, 0, 1);
  MH5_RndPrimLoad(&Uni->Pig, "BIN/MODELS/pig.obj");
} /* End of 'MH5_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitResponse( mh5UNIT_PIG *Uni, mh5ANIM *Ani )
{
  if (Ani->JPov == 0)
  {
    Uni->PigLoc =
      VecAddVec(Uni->PigLoc,
        VecMulNum(Uni->PigDir, Ani->GlobalDeltaTime * 20));
  }
  if (Ani->JPov == 4)
  {
    Uni->PigLoc =
      VecAddVec(Uni->PigLoc,
        VecMulNum(Uni->PigDir, -Ani->GlobalDeltaTime * 20));
  }
  if (Ani->JPov == 6)
  {
    Uni->PigLocOld = Uni->PigLoc;
    Uni->PigLoc = VecSet(0, 0, 0);
    Uni->P += 0.03;
    Uni->PigLoc =
      VecAddVec(Uni->PigLoc,
        VecMulNum(Uni->PigRight, -Ani->GlobalDeltaTime * 20));
  }
  if (Ani->JPov == 2)
  {
    Uni->PigLocOld = Uni->PigLoc;
    Uni->PigLoc = VecSet(0, 0, 0);
    Uni->P -= 0.03;
    Uni->PigLoc =
      VecAddVec(Uni->PigLoc,
        VecMulNum(Uni->PigRight, Ani->GlobalDeltaTime * 20));
  }
} /* End of 'MH5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitRender( mh5UNIT_PIG *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimDraw(&Uni->Pig, MatrMulMatr3(MatrScale(VecVec1(0.1)), MatrTranslate(Uni->PigLoc), MatrRotateY(Uni->P)));
  Uni->PigLoc = Uni->PigLocOld;
} /* End of 'MH5_UnitRender' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mh5UNIT *Uni;
 *   - animation context:
 *       mh5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MH5_UnitClose( mh5UNIT_PIG *Uni, mh5ANIM *Ani )
{
  MH5_RndPrimFree(&Uni->Pig);
} /* End of 'MH5_UnitClose' function */

mh5UNIT * MH5_UnitCreatePig( VOID )
{
  mh5UNIT *Uni;

  if ((Uni = MH5_AnimUnitCreate(sizeof(mh5UNIT_PIG))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MH5_UnitInit;
  Uni->Response = (VOID *)MH5_UnitResponse;
  Uni->Render = (VOID *)MH5_UnitRender;
  Uni->Close = (VOID *)MH5_UnitClose;

  return Uni;
} /* End of 'MH5_UnitCreateBall' function */

/* END OF 'u_pig.c' FILE */