/* FILE NAME   : rndprim.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing  module.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "rnd.h"

/* Primitive creation function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       mh5PRIM *Pr;
 *   - vertex attributes array:
 *       mh5VERTEX *V;
 *   - number of vertices:
 *       INT NumOfV;
 *   - index array (for trimesh – by 3 ones, may be NULL)
 *       INT *I;
 *   - number of indices
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID MH5_RndPrimCreate( mh5PRIM *Pr, mh5VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(mh5PRIM));   /* <-- <string.h> */

  if (V != NULL && NumOfV != 0)
  {
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mh5VERTEX) * NumOfV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(mh5VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(mh5VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(mh5VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(mh5VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
  }

  if (I != NULL && NumOfI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);
    Pr->NumOfElements = NumOfI;
  }
  else
    Pr->NumOfElements = NumOfV;
  Pr->Trans = MatrIdentity();
} /* End of 'MH5_RndPrimCreate' function */


VOID MH5_RndPrimFree( mh5PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);

  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(mh5PRIM));
}

VOID MH5_RndPrimDraw( mh5PRIM *Pr, MATR World )
{
  MATR wvp = MatrMulMatr3(Pr->Trans, World, MH5_RndMatrVP);

  glLoadMatrixf(wvp.A[0]);

  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  glDrawElements(GL_TRIANGLES, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

BOOL MH5_RndPrimCreateSphere( mh5PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j;
  DBL phi, theta, x, y, z;
  VEC L = {1, 1, 1};
  mh5VERTEX *V;

  if ((V = malloc(sizeof(mh5VERTEX) * SplitW * SplitH)) == NULL)
    return FALSE;

  for (i = 0, theta = 0; i < SplitH; i++, theta += PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
    {
      VEC N;
      INT nl;
      N = VecNormalize(VecMulNum(VecSubVec(V[i * SplitW + j].P, C), 1 / R));
      nl = VecDotVec(N, L);
      if (nl < 0.1)
        nl = 0.1;
      V[i].C = Vec4Set(0.8 * nl, 0.5 * nl, 0.3 * nl, 1);
      x = sin(phi) * sin(theta);
      y = cos(theta);
      z = sin(theta) * cos(phi);
      V[i * SplitW + j].P = VecSet(C.X + R * x,
                                   C.Y + R * y,
                                   C.Z + R * z);
    }
  MH5_RndPrimCreateGrid(Pr, SplitW, SplitH, V);
  free(V);
  return TRUE;
}

BOOL MH5_RndPrimCreateTor( mh5PRIM *Pr, VEC C, DBL R, DBL r, INT SplitW, INT SplitH )
{
  INT i, j;
  DBL phi, theta;
  mh5VERTEX *V;

  if ((V = malloc(sizeof(mh5VERTEX) * SplitW * SplitH)) == NULL)
    return FALSE;

  for (i = 0, theta = 0; i < SplitH; i++, theta += 2 * PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
      V[i * SplitW + j].P = VecSet(C.X + sin(phi) * (R + r * cos(theta)),
                                   C.Y + r * sin(theta),
                                   C.Z + (R + r * cos(theta)) * cos(phi));

  MH5_RndPrimCreateGrid(Pr, SplitW, SplitH, V);
  free(V);
  return TRUE;
}

BOOL MH5_RndPrimCreatePlosk( mh5PRIM *Pr, VEC C, DBL D, INT SplitW, INT SplitH )
{
  INT i, j;
  mh5VERTEX *V;

  if ((V = malloc(sizeof(mh5VERTEX) * SplitW * SplitH)) == NULL)
    return FALSE;

  for (i = 0; i < SplitH; i++)
    for (j = 0; j < SplitW; j++)
    {
      VEC4 c = {0.2, 1, 0.2, 1};
      V[i * SplitW + j].P = VecSet(C.X + i * D * (SplitH - 1) + j * D,
                                   C.Y,
                                   C.Z + j * D * (SplitW - 1) + i * D);
      V[i * SplitW + j].C = c;
    }

  MH5_RndPrimCreateGrid(Pr, SplitW, SplitH, V);
  free(V);
  return TRUE;
}

BOOL MH5_RndPrimCreateGrid( mh5PRIM *Pr, INT SplitW, INT SplitH, mh5VERTEX *V )
{
  INT i, j, k, *Ind;

  if ((Ind = malloc(sizeof(INT) * ((SplitW - 1) * (SplitH - 1) * 6))) == NULL)
    return FALSE;

  for (i = 0, k = 0; i < SplitH - 1; i++)
    for (j = 0; j < SplitW - 1; j++)
    {
      Ind[k++] = i * SplitW + j;
      Ind[k++] = i * SplitW + j + 1;
      Ind[k++] = (i + 1) * SplitW + j;

      Ind[k++] = (i + 1) * SplitW + j;
      Ind[k++] = i * SplitW + j + 1;
      Ind[k++] = (i + 1) * SplitW + j + 1;
    }

  MH5_RndPrimCreate(Pr, V, SplitH * SplitW, Ind, (SplitW - 1) * (SplitH - 1) * 6);
  free(Ind);

  return TRUE;
}

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       mh5PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       mh5PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MH5_RndPrimLoad( mh5PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT i, nv = 0, nind = 0;
  mh5VERTEX *V;
  INT *Ind;
  static CHAR Buf[1000];
  FLT nl;
  VEC L = {1, 1, 1};

  memset(Pr, 0, sizeof(mh5PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertexes and indexes */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
          n++;
      nind += (n - 2) * 3;
    }
  }

  if ((V = malloc(sizeof(mh5VERTEX) * nv + sizeof(INT) * nind)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);
  memset(V, 0, sizeof(mh5VERTEX) * nv + sizeof(INT) * nind);

  /* Load primitive */
  rewind(F);
  nv = 0;
  nind = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0, n0, n1, nc;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
        {
          sscanf(Buf + i, "%i", &nc);
          if (nc < 0)
            nc = nv + nc;
          else
            nc--;

          if (n == 0)
            n0 = nc;
          else if (n == 1)
            n1 = nc;
          else
          {
            Ind[nind++] = n0;
            Ind[nind++] = n1;
            Ind[nind++] = nc;
            n1 = nc;
          }
          n++;
        }
    }
  }

  for (i = 0; i < nv; i++)
      V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < nind; i += 3)
    {
      VEC
        p0 = V[Ind[i]].P,
        p1 = V[Ind[i + 1]].P,
        p2 = V[Ind[i + 2]].P,
        N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

      V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N); /* VecAddVecEq(&V[Ind[i]].N, N); */
      V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
      V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
    }

  for (i = 0; i < nv; i++)
  {
    V[i].N = VecNormalize(V[i].N);
    nl = VecDotVec(V[i].N, L);
    if (nl < 0.1)
      nl = 0.1;
    V[i].C = Vec4Set(0.9 * nl, 0.6 * nl, 0.8 * nl, 1);
  }

  fclose(F);
  MH5_RndPrimCreate(Pr, V, nv, Ind, nind);
  free(V);

  return TRUE;
} /* End of 'MH5_RndPrimLoad' function */

/* END OF 'rndprim.h' FILE */
