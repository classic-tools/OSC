#include "world.h"


/**************************************************************************/
/* GLOBAL **************       MakeCompound        ************************/
/**************************************************************************/
/* PURPOSE: MAKE A COMPOUND NODE.  ALL THE SUBGRAPHS HAVE BEEN BUILD AND  */
/*          ARE ADDRESSED BY glstop (IN ORDER OF ENCOUNTER). THE CURRENT  */
/*          NODE LIST ADDRESSED BY nlstop IS REMOVED TO UNCOVER THE SCOPE */
/*          DEFINING THE COMPOUND NODE AND TO COMPLETE CONSTRUCTION OF    */
/*          THE LAST SUBGRAPH.  THEN THE SUBGRAPHS ARE LINKED TO THE      */
/*          COMPOUND NODE, COMPLETING SCOPE EXIT.  THE Iterate AND        */
/*          IfThenElse NODES ARE NOT RECOGNIZED.                          */
/**************************************************************************/

void MakeCompound( label, type, cnt, lst )
int    label;
int    type;
int    cnt;
PALIST lst;
{
  register PNODE n;

  if ( (type == IFIterate) || (type == IFIfThenElse) )
    Error1( "Iterate AND IfThenElse NODEs NOT RECOGNIZED" );

  PopNodeList();

  n          = FindNode( label, type );
  n->if1line = line;

  NodeAssignPragmas( n );

  n->C_SUBS = glstop->gsucc;

  if ( glstop->gsucc != NULL )
    glstop->gsucc->gpred = n;

  n->C_SCNT = cnt;
  n->C_ALST = lst;

  AssignTags( lst, n );

  PopGraphList();
}

/* $Log: MakeCompound.c,v $
 * Revision 1.1  1993/01/21  23:29:33  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
