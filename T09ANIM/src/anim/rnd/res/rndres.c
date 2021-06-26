/* FILE NAME: rndmtl.c
 * PROGRAMMER: MH5
 * DATE: 23.06.2021
 * PURPOSE: 3D animation rendering shader handle functions module.
 */

#include "rndres.h"

VOID MH5_RndResInit( VOID )
{
  MH5_RndShadersInit();
  MH5_RndTexInit();
  MH5_RndMtlInit();
  //MH5_RndFntInit();
}

VOID MH5_RndResClose( VOID )
{
  //MH5_RndFntClose();
  MH5_RndMtlClose();
  MH5_RndTexClose();
  MH5_RndShadersClose();
}

/* END OF 'rndmtl.c' FILE */
