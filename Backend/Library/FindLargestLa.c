#include "world.h"

int LargestTypeLabelSoFar = 0;

/**************************************************************************/
/* GLOBAL **************         FindLargestLabel     *********************/
/**************************************************************************/
/* PURPOSE: RETURN THE LAST TYPE LABEL USED.                              */
/**************************************************************************/


int FindLargestLabel( )
{
  return LargestTypeLabelSoFar;
}

/* $Log: FindLargestLa.c,v $co: warning: `/* $Log' is obsolescent; use ` * $Log'.

 * Revision 1.1.1.1  2000/12/31 10:48:02  patmiller
 * LLNL distribution with Dave Raymond pthread hacks
 * (otherwise looks remarkably like it did 4 years
 * ago)
 *
 * Pat
 *
 * Revision 1.1  1994/05/25  19:44:31  denton
 * Added FindLargestLabel to return the value of LargestTypeLabelSoFar.
 * Shortened name of GetAggregateType.c to remove ar warnings.
 *
 * */
