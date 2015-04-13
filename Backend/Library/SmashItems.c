#include "world.h"

int   lclass;			/* LAST EQUIVALENCE CLASS IN htable  */
PINFO htable[MaxClass];		/* EQUIVALENCE CLASS HEAD POINTERS   */
PINFO ttable[MaxClass];		/* EQUIVALENCE CLASS TAIL POINTERS   */

/* $Log: SmashItems.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:20  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:30:24  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
