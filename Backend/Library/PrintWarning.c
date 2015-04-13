#include "world.h"


/**************************************************************************/
/* GLOBAL **************        PrintWarning       ************************/
/**************************************************************************/
/* PURPOSE: PRINT op'S COPY WARNING TO stderr.                            */
/**************************************************************************/

void PrintWarning( op, sourceline, funct, file, copy )
char *op;
int   sourceline;
char *funct;
char *file;
int   copy;
{
  if ( !Warnings ) return;	/* Are warnings suppressed? */

  if ( funct == NULL ) 
    funct = "???()";

  if ( file == NULL )  
    file = "???.sis";

  if ( copy )
    FPRINTF(stderr,
	    "%s: W - %s ON LINE %d OF %s IN %s INTRODUCES COPYING\n",
	    program, op, sourceline, funct, file
	    );
  else
    FPRINTF(stderr,
	    "%s:  W - %s ON LINE %d OF %s IN %s MAY INTRODUCE COPYING\n",
	    program, op, sourceline, funct, file
	    );
}

/* $Log: PrintWarning.c,v $
 * Revision 1.2  1993/04/16  17:09:57  miller
 * Added support for warnings suppression (-w)
 *
 * Revision 1.1  1993/01/21  23:30:03  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
