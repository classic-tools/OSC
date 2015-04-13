/* $Log: if2names.c,v $
 * Revision 1.2  1993/01/07  00:38:26  miller
 * Make changes for LINT and combined files.
 *
 * Revision 1.1  1992/12/22  23:32:04  miller
 * Merged removed files into ../Library
 * */

#include "world.h"


static char *cfun[] = {                  /* ARRAY COMPONENT COPY FUNCTION */
    "ArrArrCopy",      "Compiler_Error", "Compiler_Error",   "Compiler_Error",
    "Compiler_Error",  "Compiler_Error", "Compiler_Error",   "Compiler_Error",
    "Compiler_Error",  "Compiler_Error", "Compiler_Error",   "Compiler_Error",
    "ArrBoolCopy",     "ArrCharCopy",    "ArrDblCopy",       "ArrIntCopy",
    "ArrNullCopy",     "ArrFltCopy",     "Compiler_Error",   "Compiler_Error"
    };

static char buf[100];                        /* COPY FUNCTION NAME BUFFER */


/**************************************************************************/
/* GLOBAL **************      GetCopyFunction      ************************/
/**************************************************************************/
/* PURPOSE: RETURN COMPONENT COPY FUNCTION NAME FOR ARRAY i. NOTE, A CALL */
/*          MIGHT WIPE OUT THE PREVIOUS CONTENTS OF STRING BUFFER buf.    */
/**************************************************************************/

char *GetCopyFunction( i )
PINFO i;
{
  switch ( i->A_ELEM->type ) {
    case IF_RECORD:
    case IF_UNION:
    case IF_BRECORD:
      SPRINTF( buf, "Arr%sCopy", i->A_ELEM->cname );
      return( buf );

    default:
      ;
    }

  return( cfun[ i->A_ELEM->type ] );
}


/**************************************************************************/
/* GLOBAL **************    GetIncRefCountName     ************************/
/**************************************************************************/
/* PURPOSE: RETURN THE INCREMENT REFERENCE COUNT MACRO NAME FOR i. THE    */
/*          TYPE OF i IS ASSUMED TO BE UNION, RECORD, OR ARRAY.           */
/**************************************************************************/

char *GetIncRefCountName( i )
PINFO i;
{
  switch ( i->type ) {
    case IF_RECORD:
    case IF_UNION:
    case IF_ARRAY:
      return( "IncRefCount" );
    
    default:
      Error2( "GetIncRefCountName", "ILLEGAL TYPE" );
    }

  return NULL;
}


/**************************************************************************/
/* GLOBAL **************    GetSetRefCountName     ************************/
/**************************************************************************/
/* PURPOSE: RETURN THE SET REFERENCE COUNT MACRO NAME FOR i. THE          */
/*          TYPE OF i IS ASSUMED TO BE UNION, RECORD, OR ARRAY.           */
/**************************************************************************/

char *GetSetRefCountName( i )
PINFO i;
{
  switch ( i->type ) {
    case IF_RECORD:
    case IF_UNION:
    case IF_ARRAY:
      return( "SetRefCount" );

    default:
      Error2( "GetSetRefCountName", "ILLEGAL INFO TYPE" );
    }

  return NULL;
}
