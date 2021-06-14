/* FILE NAME: GLOBE.C
 * PROGRAMMER: MH5
 * DATE 14.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <windows.h>
#include <math.h>
#include "globe.h"
#pragma warning(disable: 4244)
#define GRID_H = 10
#define GRID_W = 10

DBL PI = 3.14159265358979323846;
VEC G[GRID_H][GRID_W];
DBL theta, phi;


VOID GlbeSet( DBL Xc, DBL Yc, DBL R )
{
  INT i, j;

  for (i = 0, theta = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      G[i][j].X = R * sin(phi) * sin(theta);
      G[i][j].Y = R * cos(theta);
      G[i][j].Z = sin(theta) * cos(phi);
    }
}

VOID GlobeDraw( HDC hDC )
{
  INT i, j;
  DBL s = 5;
  for (i = 0, theta = 0; i < GRID_H; i++, theta += (INT)(PI / (GRID_H - 1)))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * (INT)(PI / (GRID_W - 1)))
      Ellipse(hDC, G[i][j].X - s, G[i][j].Y + s, G[i][j].X + s, G[i][j].Y - s);
}

