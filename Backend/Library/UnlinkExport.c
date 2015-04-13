#include "world.h"


/**************************************************************************/
/* GLOBAL **************       UnlinkExport        ************************/
/**************************************************************************/
/* PURPOSE: UNLINK EDGE e FROM ITS SOURCE NODE'S EXPORT LIST.  IF e IS A  */
/*          CONSTANT, NOTHING IS DONE.                                    */
/**************************************************************************/

void UnlinkExport( e )
PEDGE e;
{
  if ( IsConst( e ) )
    return;

  if ( e->epred == NULL ) {	/* FIRST ONE IN LIST */
    e->src->exp = e->esucc;

    if ( e->esucc != NULL )
      e->esucc->epred = NULL;
  } else {
    e->epred->esucc = e->esucc;

    if ( e->esucc != NULL )
      e->esucc->epred = e->epred;
  }
}

/* $Log: UnlinkExport.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:21  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.3  1994/07/21  16:33:10  denton
 * Removed NULL assignment just put in to detect bad memory access.  Will fix later
 *
 * Revision 1.1  1993/01/21  23:30:36  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
