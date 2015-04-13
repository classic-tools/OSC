#include "world.h"


PNODE chead = NULL;		/* COMPOUND NODE TRUNK HEAD */
PNODE ctail = NULL;		/* COMPOUND NODE TRUNK HEAD */

PCALL cghead = NULL;		/* CALL GRAPH TRUNK HEAD */
PCALL cgtail = NULL;		/* CALL GRAPH TRUNK HEAD */

/* $Log: CallItems.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:47:59  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.2  1994/07/01  23:31:14  denton
 * Made color local.
 *
 * Revision 1.1  1993/01/21  23:28:02  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
