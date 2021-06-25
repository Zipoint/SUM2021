/* FILE NAME   : rndprim.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing  module.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "res/rndres.h"

/* Primitive creation function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       mh5PRIM *Pr;
 *   - vertex attributes array:
 *       mh5VERTEX *V;
 *   - number of vertices:
 *       INT NumOfV;
 *   - index array (for trimesh - by 3 ones, may be NULL)
 *       INT *I;
 *   - number of indices
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID MH5_RndPrimCreate( mh5PRIM *Pr, mh5PRIM_TYPE Type, mh5VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(mh5PRIM));

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
  MATR w = MatrMulMatr(Pr->Trans, World),
       winv = MatrTranspose(MatrInverse(w)),
       wvp = MatrMulMatr(w, MH5_RndMatrVP);
  INT loc;
  INT ProgId;
  INT gl_prim_type = Pr->Type == MH5_RND_PRIM_LINES ? GL_LINES :
                     Pr->Type == MH5_RND_PRIM_TRIMESH ? GL_TRIANGLES :
                     Pr->Type == MH5_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP :
                     GL_POINTS;


  glLoadMatrixf(wvp.A[0]);

  ProgId = MH5_RndShaders[0].ProgId;
  glUseProgram(ProgId);
  MH5_RndCamLoc = VecSet(10, 10, 10);

  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, MH5_Anim.Time);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, w.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrWInv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, winv.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
    glUniform3fv(loc, 1, &MH5_RndCamLoc.X);

  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
  else
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);
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
  INT i, j, *Ind, k;
  mh5VERTEX *V;
  FLT nl;
  VEC L = {1, 1, 1};

  if ((Ind = malloc(sizeof(INT) * ((SplitW - 1) * (SplitH - 1) * 6))) == NULL)
    return FALSE;

  if ((V = malloc(sizeof(mh5VERTEX) * SplitW * SplitH)) == NULL)
    return FALSE;

  for (i = 0; i < SplitH; i++)
    for (j = 0; j < SplitW; j++)
    {
      V[i * SplitW + j].P = VecSet(C.X + i * D * (SplitH - 1) + j * D,
                                   C.Y + 0.8 * sin(i) + 0.2 * cos(j),
                                   C.Z + j * D * (SplitW - 1) + i * D);
    }

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

  for (i = 0; i < SplitH * SplitW; i++)
    V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < (SplitH - 1) * (SplitW - 1) * 6; i += 3)
  {
    VEC
      p0 = V[Ind[i]].P,
      p1 = V[Ind[i + 1]].P,
      p2 = V[Ind[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

    V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
    V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
    V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
  }

  for (i = 0; i < SplitH * SplitW; i++)
  {
    V[i].N = VecNormalize(V[i].N);
    nl = VecDotVec(V[i].N, L);
    if (nl < 0.1)
      nl = 0.1;
    V[i].C = Vec4Set(0.1 * nl, 0.3 * nl, 0.13 * nl, 1);
  }

  MH5_RndPrimCreateGrid(Pr, SplitW, SplitH, V);
  free(V);
  free(Ind);
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

  MH5_RndPrimCreate(Pr, MH5_RND_PRIM_TRISTRIP, V, SplitH * SplitW, Ind, (SplitW - 1) * (SplitH - 1) * 6);
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

    V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
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
  MH5_RndPrimCreate(Pr, MH5_RND_PRIM_TRISTRIP, V, nv, Ind, nind);
  free(V);

  return TRUE;
} /* End of 'MH5_RndPrimLoad' function */

VOID MH5_RndPrimGridEvalNormals( INT SplitW, INT SplitH, mh5VERTEX *V )
{
  INT i, j;
  VEC L = {10, 10, 10};

  for (i = 0; i < SplitH * SplitW; i++)
    V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < SplitH - 1; i++)
    for (j = 0; j < SplitW - 1; j++)
    {
      mh5VERTEX
        *P00 = V + i * SplitW + j,
        *P01 = V + i * SplitW + j + 1,
        *P10 = V + (i + 1) * SplitW + j,
        *P11 = V + (i + 1) * SplitW + j + 1;
      VEC N;
 
      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                  VecSubVec(P11->P, P10->P)));
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);

      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                  VecSubVec(P00->P, P01->P)));
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }

  for (i = 0; i < SplitH * SplitW; i++)
    V[i].N = VecNormalize(V[i].N);
}


/* END OF 'rndprim.h' FILE */
