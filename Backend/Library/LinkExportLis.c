#include "world.h"


/**************************************************************************/
/* GLOBAL **************      LinkExportLists      ************************/
/**************************************************************************/
/* PURPOSE: LINK THE EXPORT LIST OF NODE n2 TO THAT OF NODE n1. NODE n2'S */
/*          EXPORT LIST IS SET TO NULL.                                   */
/**************************************************************************/

void LinkExportLists( n1, n2 )
PNODE n1;
PNODE n2;
{
  register PEDGE e;
  register PEDGE se;

  for ( e = n2->exp; e != NULL; e = se ) {
    se = e->esucc;
    LinkExport( n1, e );
  }

  n2->exp = NULL;
}

/* $Log: LinkExportLis.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:12  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/04/16  19:00:29  miller
 * Name shortening to keep the archiver from truncating names in Backend/Library
 * Since some names were changed, other files were affected.  All names in the
 * library now start with no more than 13 (unique) characters.
 *
 * Revision 1.1  1993/01/21  23:29:22  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
