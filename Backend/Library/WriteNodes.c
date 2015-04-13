#include "world.h"


/**************************************************************************/
/* GLOBAL **************        WriteNodes         ************************/
/**************************************************************************/
/* PURPOSE: PRINT THE NODES OF GRAPH g AND THEIR IMPORT EDGES TO output.  */
/*          NODE RELABELING AND (maybe) COMPOUND NODE PORT RENUMBERING IS */
/*	    DONE BEFORE PRINTING.                                         */
/**************************************************************************/

void WriteNodes( g )
PNODE g;
{
  register PNODE  n;
  register PNODE  sg;
  register PALIST l;

  switch ( g->type ) {
   case IFLGraph:
    FPRINTF( output, "G %2d", g->G_INFO->label );
    FPRINTF( output, " \"%s\"", g->G_NAME );
    break;

   case IFIGraph:
    FPRINTF( output, "I %2d", g->G_INFO->label );
    FPRINTF( output, " \"%s\"", g->G_NAME );
    break;

   case IFXGraph:
    FPRINTF( output, "X %2d", g->G_INFO->label );
    FPRINTF( output, " \"%s\"", g->G_NAME );
    break;

   case IFSGraph:
    FPRINTF( output, "G %2d", g->label );
    break;
  }

  WritePragmas( g );

  AssignNewLabels( g );
  /* WriteImports( g ); */

  for ( n = g->G_NODES; n != NULL; n = n->nsucc ) {
    switch ( n->type ) {
     case IFLoopA:
     case IFLoopB:
     case IFTagCase:
     case IFForall:
     case IFSelect:
     case IFIfThenElse:
      FPRINTF( output, "{ Compound %2d %2d\n", n->label, n->type );

      for ( sg = n->C_SUBS; sg != NULL; sg = sg->gsucc )
	WriteNodes( sg );

      FPRINTF( output, "} %2d %2d %2d   ", n->label, 
	      n->type, n->C_SCNT        );

      for ( l = n->C_ALST; l != NULL; l = l->next )
	FPRINTF( output, " %d", l->datum );

      break;

     default:
      FPRINTF( output, "N %2d %2d", n->label, n->type );
    }

    WritePragmas( n );
    WriteImports( n );
  }

  WriteImports( g ); 
}

/* $Log: WriteNodes.c,v $
 * Revision 1.2  1993/11/12  19:58:39  miller
 * Support for new IF90 compounds
 *
 * Revision 1.1  1993/01/21  23:30:53  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
