#include "world.h"


/**************************************************************************/
/* GLOBAL **************         MakeNode          ************************/
/**************************************************************************/
/* PURPOSE: MAKE A SIMPLE NODE AND INITIALIZE IT.  IF IT ALREADY EXISTS,  */
/*          THEN JUST INITIALIZE IT. THE ReplaceMulti NODE IS NOT         */
/*          RECOGNIZED.                                                   */
/**************************************************************************/

void MakeNode( label, type )
int   label;
int   type;
{
  register PNODE n;

  if ( type == IFReplaceMulti )
    Error1( "ReplaceMulti NODEs NOT RECOGNIZED" );

  n          = FindNode( label, type );
  n->if1line = line;

  NodeAssignPragmas( n );
}

/* $Log: MakeNode.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:14  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:29:44  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
