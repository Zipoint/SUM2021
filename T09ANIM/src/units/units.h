/* FILE NAME   : units.h
 * PROGRAMMER  : MH5
 * LAST UPDATE : 18.06.2021
 * PURPOSE     : 3D animation common declaration module.
 */

#include "../anim/anim.h"
#ifndef __units_h_
#define __units_h_

mh5UNIT * MH5_AnimUnitCreate( INT Size );
mh5UNIT * MH5_UnitCreateBall( VOID );
mh5UNIT * MH5_UnitCreatePlosk( VOID );
mh5UNIT * MH5_UnitCreatePig( VOID );
mh5UNIT * MH5_UnitCreateCtrl( VOID );

#endif /* __units_h_ */
/* END OF 'units.h' FILE */