/* FILE NAME: DETG.C
 * PROGRAMMER: MH5
 * DATE 14.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <stdio.h>
#include <math.h>

#include <windows.h>

#define MAX 10

typedef DOUBLE DBL;

INT N;
BOOL IsParity = TRUE;
DBL A[MAX][MAX], det;

VOID Det( VOID );
BOOL LoadMatrix( CHAR *FileName );
VOID Swap( DBL *A, DBL *B );

VOID main( VOID )
{
  LoadMatrix("IN.TXT");
  Det();
  printf("%f", det);
  getchar();
}

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  return TRUE;
}

VOID Swap( DBL *A, DBL *B )
{
  DBL tmp = *A;

  *A = *B;
  *B = tmp;
}

VOID Det( VOID )
{
  INT x, y, j, i, max_col, max_row, k;
  DBL coef;

  det = 1;

  for (i = 0; i < N; i++)
  {
    max_col = max_row = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_col = x, max_row = y;

    if (A[max_row][max_col] == 0)
    {
      det = 0;
      break;
    }

    if (max_row != i)
    {
      for (x = i; x < N; x++)
        Swap(&A[i][x], &A[max_row][x]);
      det = -det;
    }

    if (max_col != i)
    {
      for (y = 0; y < N; y++)
        Swap(&A[y][i], &A[y][max_col]);
      det = -det;
    }

    for (j = i + 1; j < N; j++)
    {
      coef = A[j][i] / A[i][i];
      A[j][i] = 0;
      for (k = i + 1; k < N; k++)
        A[j][k] -= A[i][k] * coef;
    }

    det *= A[i][i];
  }
}