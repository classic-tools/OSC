#include "world.h"


static double ElapsedTime( Start, Stop )
struct timeval *Start;
struct timeval *Stop;
{
  double Seconds;
  double USeconds;

  Seconds  = (double) (Stop->tv_sec  - Start->tv_sec);
  USeconds = (double) (Stop->tv_usec - Start->tv_usec); 

  if ( USeconds < 0.0 ) {
    USeconds += 1000000.0;
    Seconds  -= 1.0;
    }

  return( Seconds + (USeconds / 1000000.0) );
}


#ifdef CRAY 
void StartTimer() 
{ 
  (void)gettimeofday( &(MyInfo->WallTimeBuffer), (struct timezone *) NULL );
  MyInfo->CpuTime = TSECND(); 
}

void StopTimer()  
{ 
  struct timeval StopWallTime;
  register double StopCpuTime;

  StopCpuTime = TSECND(); 
  (void)gettimeofday( &StopWallTime, (struct timezone *) NULL );

  MyInfo->WallTime = ElapsedTime( &(MyInfo->WallTimeBuffer), &StopWallTime );
  MyInfo->CpuTime  = (StopCpuTime - MyInfo->CpuTime);
}

#else

#ifdef USE_TIMES
#include <sys/types.h>
#include <sys/times.h>
#ifdef SGI
#include <sys/param.h>
#endif
#else
#include <sys/resource.h>
static struct rusage StartUsage;
static struct rusage StopUsage;
#endif

#ifdef HPUXPA
#include <time.h>
clock_t clock();
#endif

void StartTimer()
{
  struct WorkerInfo *InfoPtr;
#ifdef USE_TIMES
  struct tms StartTime;
#endif
#ifdef HPUXPA
  clock_t clocks;
#endif

#ifdef ALLIANT
  InfoPtr = &(AllWorkerInfo[0]);
#else
  InfoPtr = MyInfo;
#endif

  (void)gettimeofday( &(InfoPtr->WallTimeBuffer), (struct timezone *) NULL );

#ifdef USE_TIMES
#ifdef HPUXPA
  InfoPtr->CpuTime = (double) clock ();
#else
  (void)times(&StartTime);
  InfoPtr->CpuTime = (double) (StartTime.tms_utime + StartTime.tms_stime);
#endif
#else
  getrusage( RUSAGE_SELF, &StartUsage );
#endif

}

void StopTimer()
{
  struct WorkerInfo *InfoPtr;
  struct timeval     StopWallTime;
#ifdef USE_TIMES
  struct tms         StopTime;
#endif

#ifdef ALLIANT
  InfoPtr = &(AllWorkerInfo[0]);
#else
  InfoPtr = MyInfo;
#endif


#ifdef USE_TIMES
#ifdef HPUXPA
  InfoPtr->CpuTime = (double)clock ();
#else
  (void)times(&StopTime);
  InfoPtr->CpuTime  = ((double) (StopTime.tms_utime + StopTime.tms_stime)) -
                      InfoPtr->CpuTime;
#endif
#ifdef SGI
  InfoPtr->CpuTime  /= ((double) HZ);
#else
#ifdef HPUXPA
  InfoPtr->CpuTime /=  ((double)CLOCKS_PER_SEC);
#else
  InfoPtr->CpuTime /= 60.0; /* SYSTEM DEPENDENT!!! */
#endif
#endif

#else
  getrusage( RUSAGE_SELF, &StopUsage );
  InfoPtr->CpuTime = ElapsedTime( &StartUsage.ru_utime, &StopUsage.ru_utime ) +
                     ElapsedTime( &StartUsage.ru_stime, &StopUsage.ru_stime );
#endif

  (void)gettimeofday( &StopWallTime, (struct timezone *) NULL );
  InfoPtr->WallTime = ElapsedTime( &(InfoPtr->WallTimeBuffer), &StopWallTime );
}


double TSECND()
{
  register double CurrentCpuTime;

#ifdef USE_TIMES
  struct tms         StopTime;
#else
  struct rusage StartTimerInfo;
  struct rusage StopTimerInfo;
#endif

#ifdef USE_TIMES
  (void)times(&StopTime);
  CurrentCpuTime  = ((double) (StopTime.tms_utime + StopTime.tms_stime));
#ifdef SGI
  CurrentCpuTime /= ((double) HZ);
#else
  CurrentCpuTime /= 60.0; /* SYSTEM DEPENDENT!!! */
#endif

#else
  getrusage( RUSAGE_SELF, &StopTimerInfo );

  StartTimerInfo.ru_utime.tv_sec  = 0;
  StartTimerInfo.ru_utime.tv_usec = 0;
  StartTimerInfo.ru_stime.tv_sec  = 0;
  StartTimerInfo.ru_stime.tv_usec = 0;

  CurrentCpuTime = 
	 ElapsedTime( &StartTimerInfo.ru_utime, &StopTimerInfo.ru_utime ) +
         ElapsedTime( &StartTimerInfo.ru_stime, &StopTimerInfo.ru_stime );
#endif

  return( CurrentCpuTime );
}
#endif
