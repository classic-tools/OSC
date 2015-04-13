#include "world.h"


static char *SharedBase; 
static char *SharedMemory; 
static int   SharedSize;

#ifndef RS6000
#ifndef SGI
extern char *malloc();
#endif
#endif


POINTER SharedMalloc( NumBytes )
int NumBytes;
{
  register char *ReturnPtr;

  NumBytes += 50;
  NumBytes = ALIGN( int, NumBytes );

  if ( SharedSize < NumBytes )
    SisalError( "SharedMalloc", "ALLOCATION SIZE TO BIG" );

  ReturnPtr     = SharedMemory;
  SharedMemory += NumBytes;
  SharedSize   -= NumBytes;

  return( (POINTER) ReturnPtr );
}


#ifdef ENCORE
int p_procnum = 0;
char *share_malloc();

void ReleaseSharedMemory()
{
}

void AcquireSharedMemory( NumBytes ) 
int NumBytes;
{
  SharedSize = NumBytes + 100000;

  if ( share_malloc_init( SharedSize+100000 ) != 0 )
    SisalError( "AcquireSharedMemory", "share_malloc_init FAILED" );

  SharedBase = SharedMemory = share_malloc( SharedSize-40 );

  if ( SharedMemory == (char *) NULL )
    SisalError( "AcquireSharedMemory", "share_malloc FAILED" );

  SharedMemory = ALIGN(char*,SharedMemory);
}

void StartWorkers()
{
  register int NumProcs = NumWorkers;

  while( --NumProcs > 0 )
    if ( fork() == 0 )
      break;

  EnterWorker( p_procnum = NumProcs );

  if ( NumProcs != 0 ) {
    LeaveWorker();
    exit( 0 );
    }
}

void StopWorkers()
{
  *SisalShutDown = TRUE;
  LeaveWorker();
}

void AbortParallel()
{
  (void)kill( 0, SIGKILL );
}
#endif




#if SUNIX || SUN || RS6000
int p_procnum = 0;

void ReleaseSharedMemory()
{
  free( SharedBase );
}

void AcquireSharedMemory( NumBytes ) 
int NumBytes;
{
  SharedSize = NumBytes + 100000;

  SharedBase = SharedMemory = malloc( SharedSize-40 );

  if ( SharedMemory == (char *) NULL )
    SisalError( "AcquireSharedMemory", "malloc FAILED" );

  SharedMemory = ALIGN(char*,SharedMemory);
}

void StartWorkers() 
{
  EnterWorker( p_procnum );
}

void StopWorkers()
{
  *SisalShutDown = TRUE;
  LeaveWorker();
}

void AbortParallel() 
{ 
  exit( 1 ); 
}
#endif




#ifdef ALLIANT
void ReleaseSharedMemory()
{
  free( SharedBase );
}

void AcquireSharedMemory( NumBytes ) 
int NumBytes;
{
  SharedSize = NumBytes + 100000;

  SharedBase = SharedMemory = malloc( SharedSize-40 );

  if ( SharedMemory == (char *) NULL )
    SisalError( "AcquireSharedMemory", "malloc FAILED" );

  SharedMemory = ALIGN(char*,SharedMemory);
}

void StartWorkers()
{
  EnterWorker( 0 );
}

void StopWorkers()
{
  *SisalShutDown = TRUE;
  LeaveWorker();
}

void AbortParallel() 
{ 
  (void)kill( 0, SIGKILL ); 
}
#endif




#if BALANCE || SYMMETRY
extern char *shmalloc();

void AcquireSharedMemory( NumBytes ) 
int NumBytes;
{
  SharedSize = NumBytes + 100000;

  SharedBase = SharedMemory = shmalloc( SharedSize-40 );

  if ( SharedMemory == (char *) NULL )
    SisalError( "AcquireSharedMemory", "shmalloc FAILED" );

  SharedMemory = ALIGN(char*,SharedMemory);
}

void ReleaseSharedMemory()
{
  shfree( SharedBase );
}

#ifdef GANGD
void StartWorkers()
{
  register int pID;

  begin_parallel( NumWorkers );

  GETPROCID(pID);

  EnterWorker( pID );

  if ( pID != 0 ) {
    LeaveWorker();
    end_parallel();
    }
}

void StopWorkers()
{
  *SisalShutDown = TRUE;
  LeaveWorker();
  end_parallel();
}

void AbortParallel() 
{ 
  abort_parallel(); 
}
#else
int p_procnum = 0;

void StartWorkers( )
{
  register int NumProcs = NumWorkers;

  while( --NumProcs > 0 )
    if ( fork() == 0 )
      break;

  EnterWorker( p_procnum = NumProcs );

  if ( NumProcs != 0 ) {
    LeaveWorker();
    exit( 0 );
    }
}

void StopWorkers()
{
  *SisalShutDown = TRUE;
  LeaveWorker();
}

void AbortParallel()
{
  (void)kill( 0, SIGKILL );
}
#endif

#endif




#ifdef CRAY
int TaskInfo[MAX_PROCS][3];
LOCK_TYPE TheFirstLock;

void ReleaseSharedMemory()
{
  free( SharedBase );
}

void AcquireSharedMemory( NumBytes ) 
int NumBytes;
{
  SharedSize = NumBytes + 100000;

  SharedBase = SharedMemory = malloc( SharedSize-40 );

  if ( SharedMemory == (char *) NULL )
    SisalError( "AcquireSharedMemory", "malloc FAILED" );

  SharedMemory = ALIGN(char*,SharedMemory);
}

int ProcessorId()
{
  register int pID;
  GETPROCID(pID);
  return( pID );
}

static void CrayWorker( ProcId )
int ProcId;
{
  EnterWorker( ProcId );
  LeaveWorker();
}

void StartWorkers()
{
  register int NumProcs = NumWorkers;
  register int i;

  MY_LOCKASGN;

  for ( i = 0; i < NumProcs; i++ ) {
    TaskInfo[i][0] = 3;
    TaskInfo[i][2] = i;  /* PROCESS IDENTIFIER */
    }

  for ( i = 1; i < NumProcs; i++ )
    TSKSTART( TaskInfo[i], CrayWorker, TaskInfo[i][2] );

  EnterWorker( TaskInfo[0][2] );
}

void StopWorkers()
{
  register int i;

  *SisalShutDown = TRUE;
  LeaveWorker();

  for ( i = 1; i < NumWorkers; i++ )
    TSKWAIT( TaskInfo[i] );
}

void AbortParallel()
{
  ERREXIT();
}
#endif


#ifdef SGI
static ulock_t  TheLock;
static usptr_t *UsHandle;

void ReleaseSharedMemory()
{
}

void AcquireSharedMemory( NumBytes ) 
int NumBytes;
{
  char ArenaName[50];

  SharedSize = NumBytes + 100000;

  SPRINTF( ArenaName, "/tmp/sis%d", getpid() );

  /* if ( (usconfig( CONF_INITSIZE, 1000 )) == -1 )
    SisalError( "AcquireSharedMemory", "USCONFIG CONF_INITSIZE FAILED" ); */

  if ( (usconfig( CONF_ARENATYPE, US_SHAREDONLY )) == -1 )
    SisalError( "AcquireSharedMemory", "USCONFIG CONF_ARENATYPE FAILED" );

  if ( (UsHandle = usinit(ArenaName)) == NULL)
    SisalError( "AcquireSharedMemory", "USINIT FAILED" );

  if ( (TheLock = usnewlock( UsHandle )) == (ulock_t) NULL )
    SisalError( "AcquireSharedMemory", "usnewlock FAILED" );


  SharedBase = SharedMemory = (char *) malloc( SharedSize-40 );

  if ( SharedMemory == (char *) NULL )
    SisalError( "AcquireSharedMemory", "malloc FAILED" );

  SharedMemory = ALIGN(char*,SharedMemory);
}

static void SgiTransfer( ProcId )
int ProcId;
{
  GetProcId = ProcId;

  if ( NumWorkers > 1 )
    if ( BindParallelWork )
      if ( sysmp( MP_MUSTRUN, ProcId ) == -1 )
        SisalError( "SgiTransfer", "sysmp MP_MUSTRUN FAILED" );

  EnterWorker( ProcId );

  if ( ProcId != 0 ) {
    LeaveWorker();
    _exit( 0 );
    }
}

void StartWorkers()
{
  register int NumProcs = NumWorkers;

  while( --NumProcs > 0 )
    if ( sproc( SgiTransfer, PR_SADDR, NumProcs ) == -1 )
      SisalError( "StartWorkers", "sproc FAILED" );

  if ( NumWorkers > 1 )
    if ( schedctl( SCHEDMODE, SGS_GANG, 0 ) == -1 )
      SisalError( "StartWorkers", "schedctl FAILED" );

  SgiTransfer( NumProcs );
}

void StopWorkers()
{
  *SisalShutDown = TRUE;
  LeaveWorker();
}

void AbortParallel()
{
  (void)kill( 0, SIGKILL );
}

int MyLock(plock)
register volatile LOCK_TYPE *plock;
{
  for (;;) {
    while (*(plock) == 1);
    ussetlock(TheLock);
    if (*(plock) == 0) {
      *(plock) = 1;
      usunsetlock(TheLock);
      break;
      }
    usunsetlock(TheLock);
    }
}

int MyUnlock(plock)
register volatile LOCK_TYPE *plock;
{
  *plock = 0;
}

int MyInitLock(plock)
register volatile LOCK_TYPE *plock;
{
  *plock = 0;
}

BARRIER_TYPE *MyInitBarrier()
{
  barrier_t *bar;

  if ( (bar = new_barrier( UsHandle )) == (barrier_t *) NULL )
    SisalError( "myinitbarrier", "new_barrier FAILED" );

  init_barrier(bar);

  return( (BARRIER_TYPE*) bar );
}

int MyBarrier( bar, limit )
BARRIER_TYPE *bar;
int limit;
{
  barrier( (barrier_t *) bar, limit );
}
#endif
