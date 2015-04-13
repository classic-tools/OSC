#include "world.h"

void IsPathInit( nn )
PNODE nn;
{
  register PNODE g, n;

  for ( g = nn; !IsGraph(g); g = g->npred );
  for ( n = g->nsucc; n != NULL; n = n->nsucc )
    n->checked = FALSE;
}

/* $Log: IsPathInit.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:12  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:29:14  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
