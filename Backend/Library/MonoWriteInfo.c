#include "world.h"


/**************************************************************************/
/* GLOBAL **************     MonoWriteInfo     ************************/
/**************************************************************************/
/* PURPOSE: WRITE THE INFO NODE LIST HEADED BY ihead TO OUTPUT. A NODE IS */
/*          ONLY PRINTED IF ITS PRINT FIELD HAS NOT BEEN DISABLED.        */
/**************************************************************************/

void MonoWriteInfo()
{
  register PINFO i;
  register int   t;

  for ( i = ihead; i != NULL; i = i->next ) {
    if ( !(i->print) )
      continue;

    if ( IsBasic(i) )
      t = IF_BASIC;
    else
      t = i->type;

    FPRINTF( output, "T %2d %2d", i->label, t );

    switch ( i->type ) {
     case IF_FUNCTION:
      FPRINTF( output, " %2d", (i->F_IN == NULL)? 0 : i->F_IN->label);
      FPRINTF( output, " %2d", i->F_OUT->label );
      break;

     case IF_STREAM:
     case IF_MULTIPLE:
     case IF_ARRAY:
      FPRINTF( output, " %2d   ", i->A_ELEM->label );
      break;

     case IF_UNION:
     case IF_RECORD:
      FPRINTF( output, " %2d   ", i->R_FIRST->label );
      break;

     case IF_TUPLE: 
     case IF_FIELD:
     case IF_TAG:
      FPRINTF( output, " %2d", i->L_SUB->label );
      FPRINTF( output, " %2d", 
	      (i->L_NEXT == NULL)? 0 : i->L_NEXT->label );
					 
      break;

     case IF_UNKNOWN:
      FPRINTF( output, "      " );
      break;

     default:
      FPRINTF( output, " %2d   ", i->type - BASIC_TYPE_START );
      break;
    }

    WritePragmas( i );
  }
}

/* $Log: MonoWriteInfo.c,v $
 * Revision 1.1  1993/04/16  19:00:48  miller
 * Name shortening to keep the archiver from truncating names in Backend/Library
 * Since some names were changed, other files were affected.  All names in the
 * library now start with no more than 13 (unique) characters.
 *
 * Revision 1.1  1993/01/21  23:30:51  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
