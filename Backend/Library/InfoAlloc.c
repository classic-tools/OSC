#include "world.h"


/**************************************************************************/
/* GLOBAL **************         InfoAlloc         ************************/
/**************************************************************************/
/* PURPOSE: ALLOCATE, INITIALIZE, AND RETURN AN INFO NODE.                */
/**************************************************************************/


PINFO InfoAlloc( label, type )
int   label;
int   type;
{
  register PINFO i;
  register PBBLOCK b;

  b = MyBBlockAlloc();
  i = &(b->i);

  TypeInitPragmas( i );

  i->label = label;
  i->type  = type;

  i->info1 = NULL;
  i->info2 = NULL;
  i->touch1 = FALSE;
  i->touch2 = FALSE;
  i->touch3 = FALSE;
  i->touch4 = FALSE;
  i->touch5 = FALSE;
  i->touch6 = FALSE;
  i->touchflag = FALSE;

  i->fmem   = FALSE;

  i->tname  = NULL;
  i->cname  = NULL;
  i->sname  = NULL;
  i->rname  = NULL;
  i->wname  = NULL;

  i->fname1 = NULL;
  i->fname2 = NULL;

  i->next  = NULL;
  i->print = TRUE;

  return( i );
}

/* $Log: InfoAlloc.c,v $
 * Revision 1.1  1993/01/21  23:28:56  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
