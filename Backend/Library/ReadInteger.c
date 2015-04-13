#include "world.h"


/**************************************************************************/
/* GLOBAL **************        ReadInteger        ************************/
/**************************************************************************/
/* PURPOSE: READ A UNSIGNED INTEGER, OPTIONALLY PROCEEDED BY ONE OR MORE  */
/*          BLANK OR TAB CHARACTERS.  WHEN DONE, token WILL CONTAIN THE   */
/*          CHARACTER FOLLOWING THE INTEGER. INPUT LINE BOUNDARIES ARE    */
/*          NOT CROSSED. A RUNNING RECORD OF THE LARGEST INTEGER READ IS  */
/*          KEPT IN maxint.                                               */
/**************************************************************************/

int ReadInteger()
{
  register int val = 0;

  EatSpaces;

  if ( !IsDigit( token ) )
    ReadError( "INTEGER EXPECTED" );

  while ( IsDigit( token ) ) {
    val = (val * 10)  + (token - '0');
    NextToken;
  }

  if ( maxint < val )
    maxint = val;

  return( val );
}

/* $Log: ReadInteger.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:19  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1993/01/21  23:30:10  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
