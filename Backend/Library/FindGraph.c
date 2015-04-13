#include "world.h"


/**************************************************************************/
/* GLOBAL **************         FindGraph         ************************/
/**************************************************************************/
/* PURPOSE: RETURN THE nTH GRAPH IN THE GRAPH LIST ADDRESSED BY glstop.   */
/**************************************************************************/

PNODE FindGraph( n )
int n;
{
  register PNODE g;

  for ( g = glstop->gsucc; g != NULL; g = g->gsucc, n-- )
    if ( n == 0 )
      break;

  return( g );
}

/* $Log: FindGraph.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:02  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:28:29  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
