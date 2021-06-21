/* FILE NAME: MTH.h
 * PROGRAMMER: MH5
 * DATE 15.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#ifndef __mth_h_
#define __mth_h_

#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm")

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

typedef double DBL;
typedef float FLT;
typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

/* Set matr function.
 * ARGUMENTS:
 *     vectors to be add:
 *       FLT X, Y, Z;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
                       FLT A10, FLT A11, FLT A12, FLT A13,
                       FLT A20, FLT A21, FLT A22, FLT A23,
                       FLT A30, FLT A31, FLT A32, FLT A33)
{
  MATR r =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33},
    }
  };

  return r;
} /* end of 'MatrSet' function*/

/* Set vector function.
 * ARGUMENTS:
 *     vectors to be add:
 *       FLT A, B, C;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSet( FLT A, FLT B, FLT C )
{
  VEC V;
  V.X = A;
  V.Y = B;
  V.Z = C;

  return V;
} /* end of 'VecSet' function*/

/* MatrIndetity function.
 * ARGUMENTS: none
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrIdentity( VOID )
{
  MATR I = UnitMatrix;
  return I;
} /* end of 'MatrIndetity' function*/

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  VEC tmp;
  tmp.X = V1.X + V2.X;
  tmp.Y = V1.Y + V2.Y;
  tmp.Z = V1.Z + V2.Z;
  return tmp;
} /* end of 'VecAddVec' function*/

/* VecSubVec function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  VEC tmp;
  tmp.X = V1.X - V2.X;
  tmp.Y = V1.Y - V2.Y;
  tmp.Z = V1.Z - V2.Z;
  return tmp;
} /* end of 'VecSubVec' function*/

/* VecMulNum function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1. FLT N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulNum( VEC V1, FLT N )
{
  VEC tmp;
  tmp.X = V1.X * N;
  tmp.Y = V1.Y * N;
  tmp.Z = V1.Z * N;
  return tmp;
} /* end of 'VecMulNum' function*/

/* VecDivNum function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1. FLT N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecDivNum( VEC V1, FLT N )
{
  VEC tmp;
  tmp.X = V1.X / N;
  tmp.Y = V1.Y / N;
  tmp.Z = V1.Z / N;
  return tmp;
} /* end of 'VecDivNum' function*/

/* VecNeg function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecNeg( VEC V )
{
  VEC tmp;
  tmp.X = -V.X;
  tmp.Y = -V.Y;
  tmp.Z = -V.Z;
  return tmp;
} /* end of 'VecNeg' function*/

/* VecDotVec function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (FLT) result double.
 */
__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* end of 'VecDotVec' function*/

/* VacLen function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V;
 * RETURNS:
 *   (FLT) result double.
 */
__inline FLT VecLen( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* end of 'VecLen' function*/

/* VecNormalize function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecNormalize(VEC V)
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
} /* end of 'VecNormalize' function*/

/* VecMulMatr function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V. MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];
  VEC tmp;

  tmp.X = (M.A[0][0] * V.X + M.A[1][0] * V.Y + M.A[2][0] * V.Z + M.A[3][0]) / w;
  tmp.Y = (M.A[0][1] * V.X + M.A[1][1] * V.Y + M.A[2][1] * V.Z + M.A[3][1]) / w;
  tmp.Z = (M.A[0][2] * V.X + M.A[1][2] * V.Y + M.A[2][2] * V.Z + M.A[3][2]) / w;

  return tmp;
} /* end of 'VecMulMatr' function*/

/* MatrRotate function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       FLT AngleInDegree, VEC R;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotate( FLT AngleInDegree, VEC R )
{
  FLT A = D2R(AngleInDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M = 
  {
    {
      {co + V.X * V.X * (1 - co), V.X * V.Y * (1 - co) + V.Z * si, V.X * V.Z * (1 - co) - V.Y * si, 0},
      {V.Y * V.X * (1 - co) - V.Z * si, co + V.Y * V.Y * (1 - co), V.Y * V.Z * (1 - co) + V.X * si, 0},
      {V.Z * V.X * (1 - co) + V.Y * si, V.Z * V.Y * (1 - co) - V.X * si, co + V.Z * V.Z * (1 - co), 0},
      {0, 0, 0, 1}
    }
  };

  return M;
} /* end of 'MatrRotate' function*/

/* MatrTranslate function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC T;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranslate( VEC T )
{
  MATR m =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {T.X, T.Y, T.Z, 1}
    }
  };

  return m;
} /* end of 'MatrTranslate' function*/

/* MatrMulMatr function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       MATR M1, M2;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r = {{{0}}};
  INT i, k, j;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* end of 'MatrMulMatr' function*/

/* MatrDeterm3x3 function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       FLT FLT A11, FLT A12, FLT A13,
             FLT A21, FLT A22, FLT A23,
             FLT A31, FLT A32, FLT A33;
 * RETURNS:
 *   (FLT) result double.
 */
__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                            FLT A21, FLT A22, FLT A23,
                            FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* end of 'MatrDeterm3x3' function*/

/* MatrDeterm function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       MATR M;
 * RETURNS:
 *   (FLT) result double.
 */
__inline FLT MatrDeterm( MATR M )
{
  return
    +M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                               M.A[2][1], M.A[2][2], M.A[2][3],
                               M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                               M.A[2][0], M.A[2][2], M.A[2][3],
                               M.A[3][0], M.A[3][2], M.A[3][3]) +
    +M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                               M.A[2][0], M.A[2][1], M.A[2][3],
                               M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                               M.A[2][0], M.A[2][1], M.A[2][2],
                               M.A[3][0], M.A[3][1], M.A[3][2]);
} /* end of 'MatrDeterm' function*/

/* MatrInverse function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       MATR M;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline  MATR MatrInverse( MATR M )
{
  FLT det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return MatrIdentity();

  r.A[0][0] =
    +MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
    +MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] =
    +MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2]) / det;

  return r;
} /* end of 'MatrInverse' function*/

/* MatrTranspose function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       MATR M;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranspose( MATR M )
{
  MATR m =
  {
    {
      {M.A[0][0], M.A[1][0], M.A[2][0], M.A[3][0]},
      {M.A[0][1], M.A[1][1], M.A[2][1], M.A[3][1]},
      {M.A[0][2], M.A[1][2], M.A[2][2], M.A[3][2]},
      {M.A[0][3], M.A[1][3], M.A[2][3], M.A[3][3]}
    }
  };
  return m;
} /* end of 'MatrTranspose' function*/

/* PointTransform function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V, MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M )
{
  VEC tmp;

  tmp.X = V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0];
  tmp.Y = V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1];
  tmp.Z = V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2];

  return tmp;
} /* end of 'PointTransform' function*/

/* VectorTransform function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V, MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VectorTransform( VEC V, MATR M )
{
  VEC tmp;

  tmp.X = V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0];
  tmp.Y = V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1];
  tmp.Z = V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2];

  return tmp;
} /* end of 'VectorTransform' function*/

/* MatrRotateX function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateX( FLT AngleInDegree )
{
  FLT A = D2R(AngleInDegree), si = sin(A), co = cos(A);
  MATR m =
  {
    {
      {1, 0, 0, 0},
      {0, co, si, 0},
      {0, -si, co, 0},
      {0, 0, 0, 1}
    }
  };

  return m;
} /* end of 'MatrRotateX' function*/

/* MatrRotateY function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT A = D2R(AngleInDegree), si = sin(A), co = cos(A);
  MATR m =
  {
    {
      {co, 0, -si, 0},
      {0, 1, 0, 0},
      {si, 0, co, 0},
      {0, 0, 0, 1}
    }
  };

  return m;
} /* end of 'MatrRotateY' function*/

/* MatrRotateZ function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  FLT A = D2R(AngleInDegree), si = sin(A), co = cos(A);
  MATR m =
  {
    {
      {co, si, 0, 0},
      {-si, co, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return m;
} /* end of 'MatrRotateZ' function*/

/* MatrScale function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC S;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrScale( VEC S )
{
  return MatrSet(S.X, 0, 0, 0,
                 0, S.Y, 0, 0,
                 0, 0, S.Y, 0,
                 0, 0, 0, 1);
} /* end of 'MatrScale' function*/

/* VecCrossVec function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  VEC tmp;

  tmp.X = V1.Y * V2.Z - V1.Z * V2.Y;
  tmp.Y = V1.Z * V2.X - V1.X * V2.Z;
  tmp.Z = V1.X * V2.Y - V1.Y * V2.X;

  return tmp;
} /* end of 'VecCrossVec' function*/

/* VecLen2 function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V;
 * RETURNS:
 *   (FLT) result double.
 */
__inline FLT VecLen2( VEC V )
{
  FLT t = VecLen(V);
  return t * t;
} /* end of 'VecLen2' function*/

/* VecVec1 function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       FLT A;;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecVec1( FLT A )
{
  VEC r = {A, A, A};

  return r;
}

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* Matrix look-at viewer setup function(vector).
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       FLT l, r, b, t, n, f;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrFrustum( FLT l, FLT r, FLT b, FLT t, FLT n, FLT f )
{
  MATR m =
  {
    {
      {(2 * n)/(r - l), 0, 0, 0},
      {0, (2 * n)/(t - b), 0, 0},
      {(r + l)/(r - l), (t + b)/(t - b), -(f + n)/(f - n), -1},
      {0, 0, -(2 * n * f)/(f - n), 0}
    }
  };

  return m;
} /* End of 'MatrFrustum' function */


/* Matrix look-at viewer setup function(ortho).
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrOrtho( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {2 / (Right - Left), 0, 0, 0},
      {0, 2 / (Top - Bottom), 0, 0},
      {0, 0, -2 / (Far - Near), 0},
      {-(Right + Left)/(Right - Left), -(Top + Bottom)/(Top - Bottom), -(Far + Near)/(Far - Near), 0}
    }
  };

  return m;
} /* End of 'MatrOrtho' function */

__inline VEC VecAddVec3( VEC A, VEC B, VEC C )
{
  return VecAddVec(VecAddVec(A, B), C);
}

__inline MATR MatrMulMatr3( MATR A, MATR B, MATR C)
{
  return MatrMulMatr(MatrMulMatr(A, B), C);
}

__inline MATR MatrMulMatr4( MATR A, MATR B, MATR C, MATR D )
{
  return MatrMulMatr(MatrMulMatr3(A, B, C), D);
}
#endif /* __mth_h_ */

/* END OF 'mth.h' FILE */