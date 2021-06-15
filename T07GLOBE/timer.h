/* FILE NAME: timer.h
 * PROGRAMMER: MH5
 * DATE 15.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <windows.h>
#include <time.h>

DOUBLE
  GLB_Time,
  GLB_DeltaTime,
  GLB_FPS;
BOOL GLB_IsPause;
LONG StartTime;
LONG OldTime;
LONG PauseTime;
LONG OldFPSTime;
LONG FrameCount;
