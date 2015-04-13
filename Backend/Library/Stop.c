#include "world.h"


/**************************************************************************/
/* GLOBAL **************           Stop            ************************/
/**************************************************************************/
/* PURPOSE: CLOSE ALL OPEN FILES AND STOP PROGRAM EXECUTION.  THE STATUS  */
/*          (status) IS RETURNED TO THE INVOKER OF THE PROGRAM.           */
/**************************************************************************/

void Stop( status )
int status;
{
  exit( status );
}

/* $Log: Stop.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:21  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:30:26  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
