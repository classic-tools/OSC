#include "world.h"


static void DoDependentWork()
{
  register struct ActRec *NewAR;

  if ( (NewAR = RListDeQ()) != (struct ActRec *) NULL ) {
    (*(NewAR->ChildCode))( NewAR->ArgPointer, 
			   NewAR->SliceBounds[1],
			   NewAR->SliceBounds[2],
			   NewAR->SliceBounds[0]
			  );

    NewAR->Done = TRUE;
    }
}


void Wait( Event ) 
int Event;
{
  switch ( Event ) {
    case FOR_NOTHING:
        DoDependentWork();

      break;

    case FOR_SHUTDOWN:
      while ( !(*SisalShutDown) )
          DoDependentWork();

      break;

    default:
      SisalError( "Wait", "ILLEGAL EVENT ENCOUNTERED" );
    }
}

void Sync( First, LastPlusOne )
register struct ActRec *First;
register struct ActRec *LastPlusOne;
{
  if ( First == LastPlusOne )
    return;

  for ( ; ; ) {
    while ( First->Done ) {
      First->Done = FALSE;
      First++;

      if ( First == LastPlusOne )
	return;
      }

    Wait( FOR_NOTHING );
    }
}
