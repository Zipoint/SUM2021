/* FILE NAME: DET.C
 * PROGRAMMER: MH5
 * DATE 12.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <stdio.h>

#include <windows.h>

#define MAX 10

typedef DOUBLE DBL;

INT P[MAX], N;
BOOL IsParity = TRUE;
DBL A[MAX][MAX], Det = 0, prod;

VOID Go( INT Pos );
BOOL LoadMatrix( CHAR *FileName );
VOID Swap( INT *A, INT *B );

VOID main( VOID )
{
  INT i;

  LoadMatrix("IN.TXT");
  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  printf("%f", Det);
  getchar();
}

VOID Go( INT Pos )
{
  INT i;

  if (Pos == N)
  {
    for (prod = 1, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += (IsParity * 2 - 1) * prod;
  }
  else
  {
    Go(Pos + 1);
    for (i = Pos + 1; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      IsParity = !IsParity;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
      IsParity = !IsParity;
    }
  }
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

VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
}
