#include "world.h"


/**************************************************************************/
/* GLOBAL **************      AssignNewLports      ************************/
/**************************************************************************/
/* PURPOSE: ASSIGN NEW L PORT NUMBERS, STARTING WITH p, TO THE IMPORTS    */
/*          OF LOOP l'S INITIAL SUBGRAPH AND ADJUST ALL REFERENCES. THE   */
/*          NEXT LEGAL PORT NUMBER IS RETURNED.                           */
/**************************************************************************/

int AssignNewLports( p, l )
int   p;
PNODE l;
{
  register PEDGE i;

  for ( i = l->L_INIT->imp; i != NULL; i = i->isucc, p++ )
    if ( i->iport != p ) {
      ChangeExportPorts( l->L_TEST, i->iport, -p );
      ChangeExportPorts( l->L_BODY, i->iport, -p );
      ChangeExportPorts( l->L_RET,  i->iport, -p );
      ChangeImportPorts( l->L_BODY, i->iport, -p );

      i->iport = -p;
    }

  return( p );
}

/* $Log: AssignNewLpor.c,v $
 * Revision 1.1  1993/04/16  18:59:48  miller
 * Name shortening to keep the archiver from truncating names in Backend/Library
 * Since some names were changed, other files were affected.  All names in the
 * library now start with no more than 13 (unique) characters.
 *
 * Revision 1.1  1993/01/21  23:27:52  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
