#include "world.h"


/**************************************************************************/
/* GLOBAL **************          Error2           ************************/
/**************************************************************************/
/* PURPOSE: PRINT TWO ERROR MEASAGES TO stderr AND ABORT EXECUTION.       */
/**************************************************************************/

void Error2( msg1, msg2 )
char *msg1;
char *msg2;
{
  FPRINTF( stderr, "%s: E - %s %s\n", program, msg1, msg2 );
  Stop( ERROR );
}

/* $Log: Error2.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:01  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:28:25  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
