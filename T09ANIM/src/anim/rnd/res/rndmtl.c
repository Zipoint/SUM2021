/* FILE NAME: rndmtl.c
 * PROGRAMMER: MH5
 * DATE: 23.06.2021
 * PURPOSE: 3D animation rendering shader handle functions module.
 */

#include <stdio.h>
#include <string.h>

#include "rndres.h"

mh5MATERIAL MH5_RndMaterials[MH5_MAX_MATERIALS];
INT MH5_RndMaterialsSize;

VOID MH5_RndMtlInit( VOID )
{
  INT i = 0;
  mh5MATERIAL mtl;

  MH5_RndMaterialsSize = 0;

  memset(&mtl, 0, sizeof(mtl));
  strncpy(mtl.Name, "DEFAULT", MH5_MAX_MATERIALS - 1);
  mtl.Ka = VecVec1(0.1);
  mtl.Kd = VecVec1(0.9);
  mtl.Ks = VecVec1(0.0);
  mtl.Ph = 30;
  mtl.Trans = 1;

  for (i = 0; i < 8; i++)
    mtl.Tex[i] = -1;
  mtl.ShdNo = 0;
  MH5_RndMtlAdd(&mtl);
}

VOID MH5_RndMtlClose( VOID )
{
  MH5_RndMaterialsSize = 0;
}

INT MH5_RndMtlAdd( mh5MATERIAL *Mtl )
{
  if (MH5_RndMaterialsSize >= MH5_MAX_MATERIALS)
    return -1;

  MH5_RndMaterials[MH5_RndMaterialsSize] = *Mtl;
  return MH5_RndMaterialsSize++;
}

mh5MATERIAL MH5_RndMtlGetDef( VOID )
{
  return MH5_RndMaterials[0];
}

INT MH5_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  mh5MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= MH5_RndMaterialsSize)
    MtlNo = 0;
  mtl = &MH5_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= MH5_RndShadersSize)
    prg = 0;
  prg = MH5_RndShaders[prg].ProgId;

  glUseProgram(prg);
  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, MH5_Anim.GlobalTime);
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";

    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, MH5_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
} /* End of 'MH5_RndMtlApply' function */

/* END OF 'rndmtl.c' FILE */
