#include "world.h"


/**************************************************************************/
/* GLOBAL **************        FindSource         ************************/
/**************************************************************************/
/* PURPOSE: FIND THE ACTUAL SOURCE NODE OF THE VALUE CARRIED ON EDGE e.   */
/**************************************************************************/

PEDGE FindSource( e )
register PEDGE e;
{
  register PEDGE ee;
  register PNODE src;

  for ( ;; ) {
    if ( IsConst( e ) )
      return( e );

    src = e->src;

    if ( !IsSGraph( src ) )
      return( e );

    if ( IsTagCase( src->G_DAD ) && (e->eport == 1) )
      return( e );

    if ( (ee = FindImport( src->G_DAD, e->eport )) == NULL )
      return( e );

    e = ee;
  }
}

/* $Log: FindSource.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:03  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:28:37  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
