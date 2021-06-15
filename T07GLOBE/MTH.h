/* FILE NAME: MTH.h
 * PROGRAMMER: MH5
 * DATE 15.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#ifndef __mth_h_
#define __mth_h_


#include <math.h>

#define PI = 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

typedef double DBL
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

typedef struct tagMATR
{
  DBL A[4][4];
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

void main( void )
{
}

/* Set vector function.
 * ARGUMENTS:
 *     vectors to be add:
 *       DBL X, Y, Z;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSet( DBL X, DBL Y, DBL Z)
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* end of 'VecSet' function*/

/* MatrIndetity function.
 * ARGUMENTS: none
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrIndetity( VOID )
{
  return UnitMatrix;
} /* end of 'MatrIndetity' function*/

/* MatrTranslate function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC T;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranslate( VEC T )
{
  MATR m = UnitMatrix;

  m.A[3][0] = T.X;
  m.A[3][1] = T.Y;
  m.A[3][2] = T.Z;
  return m;
} /* end of 'MatrTranslate' function*/

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
 *       VEC V1. DBL N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulNum( VEC V1, DBL N )
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
 *       VEC V1. DBL N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecDivNum( VEC V1, DBL N )
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
 *   (DBL) result double.
 */
__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* end of 'VecDotVec' function*/

/* VecLen2 function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V;
 * RETURNS:
 *   (DBL) result double.
 */
__inline DBL VecLen2( VEC V )
{
  VEC t = VecLen(V);
  return t * t;
} /* end of 'VecLen2' function*/

/* VacLen function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V;
 * RETURNS:
 *   (DBL) result double.
 */
__inline DBL VecLen( VEC V )
{
  DBL len = VecDotVec(V, V);

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
  DBL len = VecDotVec(V, V);

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
} /* end of 'VecMulMatr' function*/

/* PointTransform function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V, MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M )
{
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
} /* end of 'VectorTransform' function*/

/* MatrRotate function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       DBL AngleInDegree, VEC R;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotate( DBL AngleInDegree, VEC R )
{
  DBL A = D2R(AngleInDegree), si = sin(A), co = cos(A);
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

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* end of 'MatrMulMatr' function*/

/* MatrDeterm3x3 function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       DBL DBL A11, DBL A12, DBL A13,
             DBL A21, DBL A22, DBL A23,
             DBL A31, DBL A32, DBL A33;
 * RETURNS:
 *   (DBL) result double.
 */
__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,
                            DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* end of 'MatrDeterm3x3' function*/

/* MatrDeterm function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       MATR M;
 * RETURNS:
 *   (DBL) result double.
 */
__inline DBL MatrDeterm( MATR M )
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
  DBL det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return MatrIdentity();

  r.M[0][0] =
    +MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.M[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.M[2][0] =
    +MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.M[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.M[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.M[1][1] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.M[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.M[3][1] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.M[0][2] =
    +MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.M[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.M[2][2] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.M[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.M[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.M[1][3] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.M[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.M[3][3] =
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
} /* end of 'MatrTranspose' function*/

/* MatrRotateX function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateX( DBL AngleInDegree )
{
} /* end of 'MatrRotateX' function*/

/* MatrRotateY function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateY( DBL AngleInDegree )
{
} /* end of 'MatrRotateY' function*/

/* MatrRotateZ function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
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
} /* end of 'VecCrossVec' function*/

#endif /* __mth_h_ */
/* END OF 'mth.h' FILE */