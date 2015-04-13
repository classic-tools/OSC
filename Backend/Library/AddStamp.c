#include "world.h"


/**************************************************************************/
/* GLOBAL **************         AddStamp          ************************/
/**************************************************************************/
/* PURPOSE: ADD STAMP stamp WITH COMMENTARY s TO THE STAMP TABLE.         */
/**************************************************************************/

void AddStamp( stamp, s )  
     char stamp;
     char *s;
{ 
  stamps[ stamp ] = s;
}

/* $Log: AddStamp.c,v $
 * Revision 1.1  1993/01/21  23:27:39  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
