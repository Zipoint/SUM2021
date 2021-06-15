/* FILE NAME: GLOBE.C
 * PROGRAMMER: MH5
 * DATE 14.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <time.h>
#include <math.h>
#include <windows.h>
#include "globe.h"
#define GRID_H 8
#define GRID_W 18

DBL PI = 3.14159265358979323846;

static VEC Geom[GRID_H][GRID_W];
static INT WinW, WinH;

VEC RotateZ( VEC V, DBL Angle )
{
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = V.X * co - V.Y * si;
  r.Y = V.X * si + V.Y * co;
  r.Z = V.Z;
  return r;
}

VEC RotateY( VEC V, DBL Angle )
{
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.Y = V.Y * co - V.Z * si;
  r.Z = V.Y * si + V.Z * co;
  r.X = V.X;
  return r;
}


VOID GlobeSet( INT W, INT H, DBL R )
{
  INT i, j;
  DBL phi, theta;

  WinW = W;
  WinH = H;

  for (i = 0, theta = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      Geom[i][j].X = R * sin(phi) * sin(theta);
      Geom[i][j].Y = R * cos(theta);
      Geom[i][j].Z = sin(theta) * cos(phi);
    }
}

VOID GlobeDraw( HDC hDC )
{
  INT i, j, r, s = 3, k;
  DBL t = GLB_Time;
  static POINT pnts[GRID_H][GRID_W];

  r = WinW < WinH ? WinW : WinH;

  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC p = Geom[i][j];

      p = RotateZ(p, t * 30);
      p = RotateY(p, t * 12);

      pnts[i][j].x = WinW / 2 + p.X * 0.47  * r;
      pnts[i][j].y = WinH / 2 - p.Y * 0.47  * r;
    }

#if 0
    for (i = 0; i < GRID_H; i++)
      for (j = 0; j < GRID_W; j++)
        Ellipse(hDC, pnts[i][j].x - s, pnts[i][j].y - s, pnts[i][j].x + s, pnts[i][j].y + s);

    for (i = 0; i < GRID_H; i++)
    {
      MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
      for (j = 1; j < GRID_W; j++)
        LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
    }

    for (j = 1; j < GRID_W; j++)
    {
      MoveToEx(hDC, pnts[0][j].x, pnts[0][j].y, NULL);
      for (i = 0; i < GRID_H; i++)
        LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
    }
#endif /* 0 */
    for (k = 0; k < 1; k++)
    {
      if (k = 0)
      {
        SelectObject(hDC, GetStockObject(DC_PEN));
        SelectObject(hDC, GetStockObject(DC_BRUSH));
        SetDCPenColor(hDC, RGB(0, 0, 0));
        SetDCBrushColor(hDC, RGB(200, 200, 200));
      }
      else
      {
        SelectObject(hDC, GetStockObject(DC_PEN));
        SelectObject(hDC, GetStockObject(NULL_BRUSH));
        SetDCPenColor(hDC, RGB(255, 255, 255));
      }
      for (i = 0; i < GRID_H - 1; i++)
        for (j = 0; j < GRID_W - 1; j++)
        {
          POINT pts[4];
          INT coef;

          pts[0] = pnts[i][j];
          pts[1] = pnts[i][j + 1];
          pts[2] = pnts[i + 1][j + 1];
          pts[3] = pnts[i + 1][j];

          coef = (pts[0].x - pts[1].x) * (pts[0].y + pts[1].y) +
                 (pts[1].x - pts[2].x) * (pts[1].y + pts[2].y) +
                 (pts[2].x - pts[3].x) * (pts[2].y + pts[3].y) +
                 (pts[3].x - pts[0].x) * (pts[3].y + pts[0].y);
          if (coef <= 0 && k == 0 || coef >= 0 && k == 1)
            continue;

          Polygon(hDC, pts, 4);
      }
    }
}