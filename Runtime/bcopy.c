#if defined(HPUX)

#define SOMECODE

/* SysV version of bcopy from Eric Newton */
/* Corrected by Dae-Kyun Yoon (dkyoon@usc.edu) 12/13/91 */

extern void memcpy ();

void bcopy (source, dest, count)
char *source;
char *dest;
int count;
{
  /* No overlapping */
  if (source == dest)
    return;

  if (source < dest && source+count <= dest)
  {
     memcpy(dest, source, count);
     return;
  }

  if (dest < source && dest+count <= source)
  {
     memcpy(dest, source, count);
     return;
  }

  /* Overlap */
  /* copy forward */
  if (source > dest)
  {
    while (count-- > 0) *(dest++) = *(source++);
    return;
  }
  /* copy backward */
  if (source < dest)
  {
    count--;
    dest = dest + count;
    source = source + count;

    while (count-- >= 0) *(dest--) = *(source--);
    return;
  }
}

#endif


#if defined(SOLARIS)

#define SOMECODE

#include <string.h>

void bcopy (source, dest, count)
const char *source;
char *dest;
int count;
{
  /* No overlapping */

  if (source == dest)
    return;

  if ( (source < dest && source+count <= dest) ||
       (dest < source && dest+count <= source) )
  {
     memcpy(dest, source, count);
     return;
  }

  /* Overlap */

  memmove(dest, source, count);
  return;
}

#endif


#ifndef SOMECODE
int UNUSED_bcopy;		/* Make sure there's something... */
#endif
