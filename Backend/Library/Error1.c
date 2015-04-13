#include "world.h"


/**************************************************************************/
/* GLOBAL **************          Error1           ************************/
/**************************************************************************/
/* PURPOSE: PRINT AN ERROR MEASAGE TO stderr AND ABORT EXECUTION.         */
/**************************************************************************/

void Error1( msg1 )
char *msg1;
{
  FPRINTF( stderr, "%s: E - %s\n", program, msg1 );
  Stop( ERROR );
}

/* $Log: Error1.c,v $
 * Revision 1.1  1993/01/21  23:28:24  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
