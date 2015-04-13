#include "world.h"


/**************************************************************************/
/* GLOBAL **************        MakeInfo           ************************/
/**************************************************************************/
/* PURPOSE: MAKE AN INFO NODE AND INITIALIZE IT (LINKING IT TO OTHER INFO */
/*          NODES IF REQUIRED BY ITS TYPE. IF BUILDING A BASIC TYPE THEN  */
/*          ADJUST IT SO THAT ALL TYPE IDENTIFIERS ARE UNIQUE. THE LABEL  */
/*          IS ADJUSTED SO TO BE UNIQUE ACROSS THE MONOLITH AND THE       */
/*          LARGEST ENCOUNTERED LABEL IS REMEMBERED.                      */
/**************************************************************************/

void MakeInfo( label, type, ref1, ref2 )
int   label;
int   type;
int   ref1;
int   ref2;
{
  register PINFO i;
  register PINFO i1;
  register PINFO i2;

  switch ( type ) {
   case IF_BASIC:
    type = ref1 + BASIC_TYPE_START;
    break;

   case IF_UNKNOWN:
    break;

   default:
    i1 = FindInfo( ref1, IF_NONTYPE );
    i2 = FindInfo( ref2, IF_NONTYPE );
    break;
  }

  if ( (type == IF_DOUBLE) && flt )
    type = IF_REAL;
  else if ( (type == IF_REAL) && dbl )
    type = IF_DOUBLE;

  i = FindInfo( label, type );
  i->if1line = line;

  TypeAssignPragmas( i ); 

  i->funct = NULL;		/* DON'T BOTHER FOR TYPE INFORMATION */
  i->file  = NULL;

  switch ( type ) {
   case IF_FUNCTION:
    i->F_IN  = i1;
    i->F_OUT = i2;
    i->tname = "FUNCTION";
    break;

   case IF_STREAM:
    i->A_ELEM = i1;
    if ( !StreamsOK ) Error2( "MakeInfo", "STREAM DATA TYPE ENCOUNTERED" );
    streams = TRUE;

   case IF_MULTIPLE:
    i->A_ELEM = i1;
    i->tname  = "Multiple";
    break;

   case IF_ARRAY:
    i->A_ELEM = i1;
    i->tname = "POINTER";
    break;

   case IF_BUFFER:
    i->A_ELEM = i1;
    i->tname  = "BUFFERP"; 
    break;

   case IF_UNION:
   case IF_RECORD:
    i->tname = "POINTER";
    i->R_FIRST = i1;
    break;

   case IF_TUPLE: 
   case IF_FIELD:
   case IF_TAG:
   case IF_SET:
    i->L_SUB  = i1;
    i->L_NEXT = i2;
    i->tname  = "TupleFieldTag";
    break;

   case IF_DOUBLE:
    i->tname = "double";
    break;

   case IF_NULL:
    i->tname = "char";
    break;

   case IF_CHAR:
    i->tname = "char";
    break;

   case IF_REAL:
    i->tname = "float";
    break;

   case IF_BOOL:
    i->tname = "char";
    break;

   case IF_INTEGER:
    i->tname = "int";
    integer = i;
    break;

   default:
    i->tname = "DefaultType";
    break;
  }
}

/* $Log: MakeInfo.c,v $
 * Revision 1.2  1993/11/12  19:57:18  miller
 * Support for IF90 typeset type
 *
 * Revision 1.1  1993/01/21  23:29:37  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
