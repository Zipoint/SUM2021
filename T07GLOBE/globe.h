/* FILE NAME: GLOBE.h
 * PROGRAMMER: MH5
 * DATE 14.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#ifndef __globe_h_
#define __globe_h_

#include <stdlib.h>
#include <windows.h>
#include "timer.h"
#pragma warning(disable: 4244)
#pragma warning(disable: 4013)
#include "mth.h"

/*typedef double DBL;
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;*/

LRESULT CALLBACK MyWindowFunc( HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID FlipFullScreen( HWND hWnd );
VOID GlobeDraw( HDC hDC );
VOID GlobeSet( INT W, INT H, DBL R );
VEC RotateZ( VEC V, DBL Angle );
VEC RotateY( VEC V, DBL Angle );

#endif __globe_h_
