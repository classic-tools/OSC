#include "world.h"


/**************************************************************************/
/* GLOBAL **************          Error1           ************************/
/**************************************************************************/
/* PURPOSE: PRINT AN ERROR MESSAGE TO stderr AND ABORT EXECUTION.         */
/**************************************************************************/

void Error1( msg1 )
char *msg1;
{
  FPRINTF( stderr, "%s: E - %s\n", program, msg1 );
  Stop( ERROR );
}

/* $Log: Error1.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:01  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.3  1994/07/12  15:50:07  denton
 * Removed Warning1
 *
 * Revision 1.2  1994/06/30  17:26:46  denton
 * Add Warn1
 *
 * Revision 1.1  1993/01/21  23:28:24  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
