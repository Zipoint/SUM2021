/* FILE NAME: GLOBE.C
 * PROGRAMMER: MH5
 * DATE 14.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <time.h>
#include <math.h>
#include <windows.h>
#include "globe.h"
#define GRID_H 15
#define GRID_W 15

#define PI 3.14159265358979323846

static VEC Geom[GRID_H][GRID_W];
static INT WinW, WinH;

static DBL Power( DBL X, DBL Y )
{
  if (X < 0)
    return -pow(-X, Y);
  return pow(X, Y);
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
      DBL a = 0.25, b = 0.25;
      Geom[i][j].X = R * Power(sin(phi), a) * Power(sin(theta), b);
      Geom[i][j].Y = R * Power(cos(theta), a) * 2;
      Geom[i][j].Z = Power(sin(theta), a) * Power(cos(phi), b);
    }
}

VOID GlobeDraw( HDC hDC )
{
  INT i, j, r, s = 3, k;
  DBL t = GLB_Time;
  MATR m;
  DBL Hp, Wp, size = 2, ProjDist;
  static POINT pnts[GRID_H][GRID_W];

    Wp = Hp = size;
  if (WinW > WinH)
    Wp *= (DBL)WinW / WinH;
  else
    Hp *= (DBL)WinH / WinW;
  ProjDist = size;

  r = WinW < WinH ? WinW : WinH;

  m = MatrMulMatr3(MatrRotateZ(t * 30), MatrRotateY(t * 18), MatrTranslate(VecSet(0, fabs(0.8 * sin(t * 5)), 0)));
  m = MatrMulMatr(m, MatrView(VecSet(3 + fabs(sin(GLB_Time)) * 10, 0, sin(GLB_Time) * 5), VecSet(0, 0, 0), VecSet(0, 1, 0)));
  m = MatrMulMatr(m, MatrFrustum(-Wp / 2, Wp / 2, -Hp / 2, Hp / 2, ProjDist, 300));

  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC v = VecMulMatr(Geom[i][j], m); //(Geom[i][j], m);

      //v.X = v.X * ProjDist / -v.Z;
      //v.Y = v.Y * ProjDist / -v.Z;

      pnts[i][j].x = (v.X + 1) * WinW / 2; //v.X * WinW / Wp + WinW / 2;
      pnts[i][j].y = (-v.Y + 1) * WinH / 2; //-v.Y * WinH / Hp + WinH / 2;
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
      /* if (k == 0)
      {
        SelectObject(hDC, GetStockObject(DC_PEN));
        SelectObject(hDC, GetStockObject(DC_BRUSH));
        SetDCPenColor(hDC, RGB(0, 0, 0));
        SetDCBrushColor(hDC, RGB(255, 255, 0));
      }
      else
      {
        SelectObject(hDC, GetStockObject(DC_PEN));
        SelectObject(hDC, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hDC, RGB(255, 0, 0));
        SetDCPenColor(hDC, RGB(255, 255, 255));
      } */

      SelectObject(hDC, GetStockObject(NULL_PEN));
      SelectObject(hDC, GetStockObject(DC_BRUSH));

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

          if ((i + j) % 2 == 0)
            SetDCBrushColor(hDC, RGB(255, 255, 255));
          else
            SetDCBrushColor(hDC, RGB(0, 0, 0));

          Polygon(hDC, pts, 4);
      }
    }
}

/* END OF 'globe.c' FILE */