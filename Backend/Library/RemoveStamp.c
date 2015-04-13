#include "world.h"


/**************************************************************************/
/* GLOBAL **************        RemoveStamp        ************************/
/**************************************************************************/
/* PURPOSE: REMOVE STAMP stamp FROM THE STAMP TABLE.                      */
/**************************************************************************/

void RemoveStamp( stamp )
     char stamp;
{
  stamps[ stamp ] = NULL;
}

/* $Log: RemoveStamp.c,v $
 * Revision 1.1  1993/01/21  23:30:21  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
