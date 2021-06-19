/* FILE NAME   : anim.h
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "rnd/rnd.h"
#ifndef __anim_h_
#define __anim_h_

#define MH5_MAX_UNITS 3000
#define UNIT_BASE_FIELDS \
  VOID (*Init)( mh5UNIT *Uni, mh5ANIM *Ani );      \
  VOID (*Close)( mh5UNIT *Uni, mh5ANIM *Ani );     \
  VOID (*Response)( mh5UNIT *Uni, mh5ANIM *Ani );  \
  VOID (*Render)( mh5UNIT *Uni, mh5ANIM *Ani )

typedef struct tagmh5UNIT mh5UNIT;
typedef struct tagmh5ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;

  mh5UNIT *Units[MH5_MAX_UNITS];
  INT NumOfUnits;
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;                     /* Pause flag */
  BYTE
    JBut[32], JButOld[32], JButClick[32]; /* Joystick button states */
  INT JPov;                               /* Joystick point-of-view control [-1,0..7] */
  DBL
    JX, JY, JZ, JR;                       /* Joystick axes */
  INT Mx, My, Mz, Mdx, Mdy, Mdz; /* Mouse data */
  BYTE Keys[256];
  BYTE KeysClick[256];
  BYTE KeysOld[256];
} mh5ANIM;

struct tagmh5UNIT
{
  UNIT_BASE_FIELDS;
};

extern mh5ANIM MH5_Anim;

VOID MH5_AnimInit( HWND hWnd );
VOID MH5_AnimClose( VOID );
VOID MH5_AnimResize( INT W, INT H );
VOID MH5_AnimCopyFrame( HDC hDC );
VOID MH5_AnimRender( VOID );
VOID MH5_AnimAddUnit( mh5UNIT *Uni );
VOID MH5_AnimFlipFullScreen( VOID );
VOID MH5_AnimExit( VOID );
VOID MH5_TimerInit( VOID );
VOID MH5_TimerResponse( VOID );
VOID MH5_AnimInputResponse( VOID );
VOID MH5_AnimInputInit( VOID );


#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */