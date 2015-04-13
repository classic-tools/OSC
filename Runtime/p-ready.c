#include "world.h"


static struct ActRecCache *ARList;


void InitReadyList()
{
  register int Index;

  ARList = (struct ActRecCache *) 
	   SharedMalloc( SIZEOF(struct ActRecCache) * NumWorkers );

  for ( Index = 0; Index < NumWorkers; Index++ ) {
    MY_INIT_LOCK( (&(ARList[Index].Mutex)) );
    ARList[Index].Head = (struct ActRec *) NULL;
    ARList[Index].Tail = (struct ActRec *) NULL;
    }
}

#define DoTheEnQ( ThisAR, LastAR, Where ) \
{ \
  register struct ActRecCache *arc; \
  arc = Where; \
  LastAR->NextAR = (struct ActRec *) NULL; \
  MY_LOCK( &(arc->Mutex) ); \
  if ( arc->Head != (struct ActRec *) NULL ) { \
    arc->Tail->NextAR = ThisAR; \
    arc->Tail = LastAR; \
  } else { \
    arc->Head = ThisAR; \
    arc->Tail = LastAR; \
    } \
  MY_UNLOCK( &(arc->Mutex) ); \
}


void RListEnQ( FirstAR, LastAR )
register struct ActRec *FirstAR;
register struct ActRec *LastAR;
{
  register unsigned  int  NextPid;
  register struct ActRec *CurrAR;
  register struct ActRec *EndAR;

  if ( BindParallelWork ) {
    NextPid = 0;
    CurrAR  = FirstAR;
    EndAR   = LastAR + 1;

    while (CurrAR != EndAR) {
      DoTheEnQ( CurrAR, CurrAR, &(ARList[NextPid]) );

      CurrAR++;
      NextPid++;

      if ( NextPid == NumWorkers )
        NextPid = 0;
      }

    return;
    }

  DoTheEnQ( FirstAR, LastAR, &(ARList[0]) );
}


struct ActRec *RListDeQ()
{
  register struct ActRec      *ThisAR;
  register struct ActRecCache *arc;
  register int                 pID;

  if ( BindParallelWork ) {
    GETPROCID(pID);
    arc = &(ARList[pID]);
    }
  else
    arc = &(ARList[0]);

  if ( arc->Head == (struct ActRec *) NULL )
    return( (struct ActRec *) NULL );

  MY_LOCK( &(arc->Mutex) );

  ThisAR = arc->Head;

  if( ThisAR != (struct ActRec *) NULL )
    arc->Head = ThisAR->NextAR;

  MY_UNLOCK( &(arc->Mutex) );

  return( ThisAR );
}
