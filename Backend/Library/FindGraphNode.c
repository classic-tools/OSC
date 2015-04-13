#include "world.h"


/**************************************************************************/
/* GLOBAL **************      FindGraphNode        ************************/
/**************************************************************************/
/* PURPOSE: RETURN THE GRAPH NODE OWNING NODE n.                          */
/**************************************************************************/

PNODE FindGraphNode( n )
PNODE n;
{
  register PNODE nn;

  for ( nn = n; !IsGraph(nn); nn = nn->npred ) 
    /* DO NOTHING */;

  return( nn );
}

/* $Log: FindGraphNode.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:02  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:28:30  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
