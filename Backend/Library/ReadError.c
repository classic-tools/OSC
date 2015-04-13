#include "world.h"


/**************************************************************************/
/* GLOBAL **************         ReadError         ************************/
/**************************************************************************/
/* PURPOSE: PRINT msg TO stderr ALONG WITH THE LINE NUMBER OF THE LAST    */
/*          LINE READ AND ABORT EXECUTION.                                */
/**************************************************************************/

void ReadError( msg )
char *msg;
{
  FPRINTF( stderr, "%s: -E (line %d) %s\n", program, line, msg );
  Stop( ERROR );
}

/* $Log: ReadError.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:19  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:30:09  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
