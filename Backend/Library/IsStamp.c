#include "world.h"


/**************************************************************************/
/* GLOBAL **************          IsStamp          ************************/
/**************************************************************************/
/* PURPOSE: RETURNS TRUE IF STAMP stamp IS DEFINED, ELSE FALSE.           */
/**************************************************************************/

int  IsStamp( stamp )
char stamp;
{
  if ( stamps[ stamp ] == NULL )
    return( FALSE );

  return( TRUE );
}

/* $Log: IsStamp.c,v $
 * Revision 1.1  1993/01/21  23:29:16  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
