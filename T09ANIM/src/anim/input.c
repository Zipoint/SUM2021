/* FILE NAME: input.c
 * PROGRAMMER: MH5
 * DATE 15.06.2021
 * PURPOSE: WinAPI windowed application sample.
 */

#include <string.h>
#include "anim.h"

#define MH5_GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

POINT pt;
extern MH5_MouseWheel;

static VOID MH5_AnimKeyboardInit( VOID )
{
  INT i;

  GetKeyboardState(MH5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    MH5_Anim.Keys[i] >>= 7;
    MH5_Anim.KeysClick[i] = MH5_Anim.Keys[i] && !MH5_Anim.KeysOld[i];
  }
}

static VOID MH5_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(MH5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    MH5_Anim.Keys[i] >>= 7;
    MH5_Anim.KeysClick[i] = MH5_Anim.Keys[i] && !MH5_Anim.KeysOld[i];
  }
}

static VOID MH5_AnimMouseInit( VOID )
{
  GetCursorPos(&pt);
  ScreenToClient(MH5_Anim.hWnd, &pt);
}

static VOID MH5_AnimMouseResponse( VOID )
{
  GetCursorPos(&pt);
  ScreenToClient(MH5_Anim.hWnd, &pt);

  MH5_Anim.Mdx = pt.x - MH5_Anim.Mx;
  MH5_Anim.Mdy = pt.y - MH5_Anim.My;

  MH5_Anim.Mx = pt.x;
  MH5_Anim.My = pt.y;

  MH5_Anim.Mdz = MH5_MouseWheel;
  MH5_Anim.Mz += MH5_MouseWheel;
  MH5_MouseWheel = 0;
}

static VOID MH5_AnimJoystickInit( VOID )
{
    INT i;

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          MH5_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          MH5_Anim.JButClick[i] = MH5_Anim.JBut[i] && !MH5_Anim.JButOld[i];
          MH5_Anim.JButOld[i] = MH5_Anim.JBut[i];
        }
        /* Axes */
        MH5_Anim.JX = MH5_GET_JOYSTIC_AXIS(X);
        MH5_Anim.JY = MH5_GET_JOYSTIC_AXIS(Y);
        MH5_Anim.JZ = MH5_GET_JOYSTIC_AXIS(Z);
        MH5_Anim.JR = MH5_GET_JOYSTIC_AXIS(R);
        /* Point of view */
        MH5_Anim.JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
}

static VOID MH5_AnimJoystickResponse( VOID )
{
  INT i;

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;


      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          MH5_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          MH5_Anim.JButClick[i] = MH5_Anim.JBut[i] && !MH5_Anim.JButOld[i];
          MH5_Anim.JButOld[i] = MH5_Anim.JBut[i];
        }
        /* Axes */
        MH5_Anim.JX = MH5_GET_JOYSTIC_AXIS(X);
        MH5_Anim.JY = MH5_GET_JOYSTIC_AXIS(Y);
        MH5_Anim.JZ = MH5_GET_JOYSTIC_AXIS(Z);
        MH5_Anim.JR = MH5_GET_JOYSTIC_AXIS(R);
        /* Point of view */
        MH5_Anim.JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
}

VOID MH5_AnimInputInit( VOID )
{
MH5_AnimKeyboardInit();
MH5_AnimMouseInit();
MH5_AnimJoystickInit();
}

VOID MH5_AnimInputResponse( VOID )
{
MH5_AnimKeyboardResponse();
MH5_AnimMouseResponse();
MH5_AnimJoystickResponse();
}

/* END OF 'input.c' FILE */