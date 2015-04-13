#include "world.h"

/**************************************************************************/
/* GLOBAL **************       AddTimeStamp        ************************/
/**************************************************************************/
/* PURPOSE: Add a stamp with the current time of day to the list of IFx   */
/*	    Stamps to display.						  */
/**************************************************************************/

void AddTimeStamp()
{
  struct timeval timeofday;
  char		 *asciitime;
  char		 *p;

  gettimeofday(&timeofday,NULL);

  asciitime = ctime(&timeofday.tv_sec);
  for(p=asciitime;*p;p++) if ( *p == '\n' ) { *p = '\0'; break; }

  AddStamp('T',asciitime);
}

/* $Log: AddTimeStamp.c,v $
 * Revision 1.3  1994/04/01  00:02:42  denton
 * NULL -> '\0' where appropriate
 *
 * Revision 1.2  1993/03/23  22:51:32  miller
 * date problem
 *
 * Revision 1.1  1994/03/11  18:23:31  miller
 * Added time stamp to all output IF1 and IF2
 * */
