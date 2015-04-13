#include "world.h"


/**************************************************************************/
/* GLOBAL **************      AssignNewLabels      ************************/
/**************************************************************************/
/* PURPOSE: ASSIGN NEW LABELS TO THE NODES OF GRAPH g AND FIX SIGNED      */
/*          ARITHMETIC CONSTANTS (GIVEN sgnok IS TRUE).                   */
/**************************************************************************/

void  AssignNewLabels( g )
register PNODE g;
{
  register int   lab;
  register PNODE n;

  if ( FixPortsToo ) {
    /*************************************************************************/
    /***************      AssignNewLabelsAndPorts      ***********************/
    /*************************************************************************/
    /* PURPOSE: ASSIGN NEW LABELS TO THE NODES OF GRAPH g AND ASSIGN NEW PORT*/
    /*          NUMBERS TO THE K, M, T, L, AND R PORTS OF ALL COMPOUND NODES */
    /*          SUCH THAT NODE LABELS BEGIN WITH 1 (ASSIGNED IN INCREMENTS OF*/
    /*          ONE) AND PORT NUMBERS OBAY THE FOLLOWING:                    */
    /*                                                                       */
    /*          FORALL:  K < M < T  AND K AND R PORTS  START AT 1 BY 1       */
    /*          LOOPA :  K < L < T  AND K AND R PORTS  START AT 1 BY 1       */
    /*          LOOPB :  K < L      AND K AND R PORTS  START AT 1 BY 1       */
    /*                                                                       */
    /*          NOTE: CONSTANT FOLDING SIDE EFFECTS ARE UNDONE.              */
    /*************************************************************************/
    register int   p;

    FixConstantImports( g, 0 );

    for ( lab = 0, n = g->G_NODES; n != NULL; n = n->nsucc ) {
      lab = FixConstantImports( n, lab );

      n->label = ++lab;

      if ( !IsCompound( n ) )
	continue;

      p = AssignNewKports( n );

      switch ( n->type ) {
       case IFTagCase:
	for ( g = n->C_SUBS; g != NULL; g = g->gsucc )
	  AssignNewRports( g );

	break;

       case IFSelect:
	n->S_TEST->imp->iport = 1; /* B PORT NUMBER */

	AssignNewRports( n->S_ALT );
	AssignNewRports( n->S_CONS );
	break;

       case IFForall:
	(void)AssignNewTports( AssignNewMports( p, n ), n );
	AssignNewRports( n->F_RET );
	break;

       case IFLoopA:
	n->L_TEST->imp->iport = 1; /* B PORT NUMBER */

	(void)AssignNewLoopTports( AssignNewLports( p, n ), n );
	AssignNewRports( n->L_RET );
	break;

       case IFLoopB:
	n->L_TEST->imp->iport = 1; /* B PORT NUMBER */

	AssignNewLports( p, n );
	AssignNewRports( n->L_RET );
	break;
      }
    }
  } else {
    FixSignedConstantImports( g, 0 );

    for ( lab = 0, n = g->G_NODES; n != NULL; n = n->nsucc ) {
      lab = FixSignedConstantImports( n, lab );

      n->label = ++lab;
    }
  }
}

/* $Log: AssignNewLabe.c,v $
 * Revision 1.1  1993/04/16  18:59:45  miller
 * Name shortening to keep the archiver from truncating names in Backend/Library
 * Since some names were changed, other files were affected.  All names in the
 * library now start with no more than 13 (unique) characters.
 *
 * Revision 1.1  1993/01/21  23:27:50  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
