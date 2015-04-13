#include "world.h"


volatile int      *SisalShutDown;
struct WorkerInfo *AllWorkerInfo;

BARRIER_TYPE      *FinishBarrier;
BARRIER_TYPE      *StartBarrier;


void InitWorkers()
{
  int    WorkerCount;
  struct WorkerInfo *InfoPtr;
  int    BytesNeeded;

   SisalShutDown = (int *) SharedMalloc( SIZEOF(int) );
  *SisalShutDown = FALSE;

  if ( NumWorkers > 1 ) {
    FinishBarrier = (BARRIER_TYPE*) SharedMalloc( SIZEOF(BARRIER_TYPE) );
    INIT_BARRIER(FinishBarrier,NumWorkers);

    StartBarrier  = (BARRIER_TYPE*) SharedMalloc( SIZEOF(BARRIER_TYPE) );
    INIT_BARRIER(StartBarrier,NumWorkers);
    }

  BytesNeeded   = SIZEOF(struct WorkerInfo) * NumWorkers;
  AllWorkerInfo = (struct WorkerInfo *) SharedMalloc( BytesNeeded );

#ifndef ALLIANT
#ifndef CRAY
#ifndef SGI
  p_procnum = 0;  /* FOR FIBRE PROCESSING??? */
#endif
#endif
#endif

#ifdef SGI
  GetProcId = 0;
#endif

  for ( WorkerCount = NumWorkers ; WorkerCount > 0; WorkerCount-- ) {
    InfoPtr = &(AllWorkerInfo[ WorkerCount - 1 ]);

    InfoPtr->FlopInfo       = 0.0;
    InfoPtr->FlopCountA     = 0.0;
    InfoPtr->FlopCountL     = 0.0;
    InfoPtr->FlopCountI     = 0.0;

    InfoPtr->CopyInfo       = 0.0;
    InfoPtr->RBuilds        = 0.0;
    InfoPtr->ATAttempts     = 0.0;
    InfoPtr->ATCopies       = 0.0;
    InfoPtr->ANoOpAttempts  = 0.0;
    InfoPtr->ANoOpCopies    = 0.0;
    InfoPtr->RNoOpAttempts  = 0.0;
    InfoPtr->RNoOpCopies    = 0.0;
    InfoPtr->ADataCopies    = 0.0;

    InfoPtr->DsaHelp        = 0;
    InfoPtr->StorageUsed    = 0;
    InfoPtr->StorageWanted  = 0;
    }
}


void LeaveWorker()
{
  if ( GatherPerfInfo )
    StopTimer();

  if ( NumWorkers > 1 ) {
    WAIT_BARRIER(FinishBarrier);
    }
}

void EnterWorker( ProcId )
int ProcId;
{
  if ( NumWorkers > 1 ) {
    WAIT_BARRIER(StartBarrier);
    }

  if ( GatherPerfInfo )
    StartTimer();

  if ( ProcId != 0 )
    Wait( FOR_SHUTDOWN );
}
