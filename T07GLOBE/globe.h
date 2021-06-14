/* FILE NAME: GLOBE.h
 * PROGRAMMER: MH5
 * DATE 14.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <windows.h>
#include <stdlib.h>

typedef double DBL;
typedef struct
{
  DBL X, Y, Z;
} VEC;

LRESULT CALLBACK MyWindowFunc( HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID FlipFullScreen( HWND hWnd );
VOID GlobeDraw( HDC hDC );
VOID GlbeSet( DBL Xc, DBL Yc, DBL R );