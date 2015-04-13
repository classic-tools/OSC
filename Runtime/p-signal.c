#include "world.h"


#ifdef CRAY

static void SignalHandler( SigCode )
int SigCode;
{
  extern void InitSignalSystem();

  if ( UsingSdbx )
    switch ( SigCode ) {
      case SIGINT:
	SdbxMonitor( SDBX_INT );
	InitSignalSystem();
	return;

      case SIGFPE:
	SdbxMonitor( SDBX_FPE );
	break;

      default:
	break;
      }

  FPRINTF( stderr, "\nERROR: SignalCode=%d\n", SigCode );
  AbortParallel();
}

void InitSignalSystem()
{
  signal( SIGINT, SignalHandler );
  signal( SIGFPE, SignalHandler );
}

#else

static int SignalHandler( SigCode )
int SigCode;
{
  extern void InitSignalSystem();

  if ( UsingSdbx )
    switch( SigCode ) {
      case SIGINT:
	SdbxMonitor( SDBX_INT );
	InitSignalSystem();
	return;

      case SIGFPE:
	SdbxMonitor( SDBX_FPE );
	break;

      default:
	break;
      }

  psignal( SigCode, "\nERROR"  );
  AbortParallel();
}

void InitSignalSystem()
{
     /* DYNIX V2.1: shbrk and shsbrk require default SIGSEGV handler for    */
     /*             internal use (extension of shared memory)               */
     /*             Since shmalloc only called once from virtual processor  */
     /*             0, handler for segmentation faults can be used if setup */
     /*             after the shmalloc call (which is the case)             */
     /*             This is not done by libppp.a                            */

  (void)signal( SIGHUP,   SignalHandler );
  (void)signal( SIGINT,   SignalHandler );
  (void)signal( SIGQUIT,  SignalHandler );
  (void)signal( SIGILL,   SignalHandler );
  (void)signal( SIGFPE,   SignalHandler );
  (void)signal( SIGBUS,   SignalHandler );
  (void)signal( SIGPIPE,  SignalHandler );
#ifndef HPUX
#ifndef HPUXPA
  (void)signal( SIGXCPU,  SignalHandler );
  (void)signal( SIGXFSZ,  SignalHandler );
  (void)signal( SIGSEGV,  SignalHandler );
#endif
#endif
}
#endif
