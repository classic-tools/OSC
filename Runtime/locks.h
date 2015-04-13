extern void MyInitLock();
extern void MyLock();
extern void MyUnlock();
extern void MyBarrier();

#if !defined(NON_COHERENT_CACHE) || !defined(POWER4)
#define FLUSHALL
#define FLUSH(start,end)
#define FLUSHLINE(addr)
#define CACHESYNC
#define FFL
#endif

#ifdef ENCORE
#include <parallel.h>
typedef LOCK          LOCK_TYPE;
typedef BARRIER       BARRIER_TYPE;

#define MY_SLOCK(lp)          spin_lock((lp))
#define MY_SUNLOCK(lp)        spin_unlock((lp))
#define MY_SINIT_LOCK(lp)     spin_unlock((lp))

#define MY_LOCK(lp)           spin_lock((lp))
#define MY_UNLOCK(lp)         spin_unlock((lp))
#define MY_INIT_LOCK(lp)      spin_unlock((lp))

#define MY_LOCK_BACKUP(lp)    spin_lock((lp))
#define MY_UNLOCK_BACKUP(lp)  spin_unlock((lp))

#define INIT_BARRIER(bp,limit) \
{ \
  (bp) = barrier_init( (BARRIER_TYPE*) SharedMalloc( SIZEOF(BARRIER_TYPE) ), \
		      limit, SPIN_BLOCK ); \
  if ( (bp) == NULL ) \
    SisalError( "INIT_BARRIER", "barrier_init FAILED" ); \
}

#define WAIT_BARRIER(bp) barrier(bp)
#endif


#ifdef ALLIANT
typedef unsigned char LOCK_TYPE;
typedef int           BARRIER_TYPE;

#define MY_SLOCK(lp)          lock((lp))
#define MY_SUNLOCK(lp)        unlock((lp))
#define MY_SINIT_LOCK(lp)     initialize_lock((lp))

#define MY_LOCK(lp)           lock((lp))
#define MY_UNLOCK(lp)         unlock((lp))
#define MY_INIT_LOCK(lp)      initialize_lock((lp))

#define MY_LOCK_BACKUP(lp)    lock((lp))
#define MY_UNLOCK_BACKUP(lp)  unlock((lp))

#define INIT_BARRIER(b,limit)
#define WAIT_BARRIER(b)
#endif



#ifdef CRAY
typedef int LOCK_TYPE;
typedef int BARRIER_TYPE;

extern LOCK_TYPE TheFirstLock;

#define MY_SLOCK(lp)          while ( LOCKTEST(lp) != 0 )
#define MY_SUNLOCK(lp)        LOCKOFF(lp)
#define MY_SINIT_LOCK(lp)     LOCKASGN(lp)

#ifdef CRAYXY
#define MY_LOCK(lp)           _semts(31)
#define MY_UNLOCK(lp)         { _cmr(); _semclr(31); }
#define MY_INIT_LOCK(lp)
#define MY_LOCK_BACKUP(lp)    _semts(31)
#define MY_UNLOCK_BACKUP(lp)  { _cmr(); _semclr(31); }
#define MY_LOCKASGN           { LOCKASGN(&TheFirstLock); _semclr(31); }
#endif

#ifdef CRAY2
#define MY_LOCK(lp)           _getsem()
#define MY_UNLOCK(lp)         _csm()
#define MY_INIT_LOCK(lp)
#define MY_LOCK_BACKUP(lp)    _getsem()
#define MY_UNLOCK_BACKUP(lp)  _csm()
#define MY_LOCKASGN           { LOCKASGN(&TheFirstLock); _csm(); }
#endif

#ifdef CRAYT3D
#define MY_LOCK(lp)           while ( LOCKTEST(&TheFirstLock) != 0 )
#define MY_UNLOCK(lp)         LOCKOFF(&TheFirstLock)
#define MY_INIT_LOCK(lp)
#define MY_LOCK_BACKUP(lp)    while ( LOCKTEST(&TheFirstLock) != 0 )
#define MY_UNLOCK_BACKUP(lp)  LOCKOFF(&TheFirstLock)
#define MY_LOCKASGN           { LOCKASGN(&TheFirstLock); }
#endif

#define INIT_BARRIER(b,limit)  BARASGN(b,&limit)
#define WAIT_BARRIER(b)        BARSYNC(b)
#endif



#if defined(SUNIX) || defined(SUN) || (defined(RS6000) && !defined(POWER4))
typedef unsigned char LOCK_TYPE;
typedef int           BARRIER_TYPE;

#define	MY_SLOCK(lp) {if (*lp == 'L') SisalError("","MY_SLOCK FAILED"); *lp = 'L'; }
#define MY_SUNLOCK(lp) {*lp = 'U';}
#define MY_SINIT_LOCK(lp) {*lp = 'U';}

#define	MY_LOCK(lp) {if (*lp == 'L') SisalError("","MY_LOCK FAILED"); *lp = 'L'; }
#define MY_UNLOCK(lp) {*lp = 'U';}
#define MY_INIT_LOCK(lp) {*lp = 'U';}

#define MY_LOCK_BACKUP(lp) {if (*lp == 'L') SisalError("","MY_LOCK_BACKUP FAILED"); *lp = 'L';}
#define MY_UNLOCK_BACKUP(lp) {*lp = 'U';}

#define INIT_BARRIER(b,lm)
#define WAIT_BARRIER(b)
#endif




#if SYMMETRY || BALANCE
#include <parallel/parallel.h>
typedef slock_t    LOCK_TYPE;
typedef sbarrier_t BARRIER_TYPE;

#define MY_SLOCK(lp)          s_lock((lp))
#define MY_SUNLOCK(lp)        s_unlock((lp))
#define MY_SINIT_LOCK(lp)     s_init_lock((lp))

#define MY_LOCK(lp)           s_lock((lp))
#define MY_UNLOCK(lp)         s_unlock((lp))
#define MY_INIT_LOCK(lp)      s_init_lock((lp))

#define MY_LOCK_BACKUP(lp)    s_lock((lp))
#define MY_UNLOCK_BACKUP(lp)  s_unlock((lp))

#define INIT_BARRIER(b,np)    s_init_barrier(b,np)
#define WAIT_BARRIER(b)       s_wait_barrier(b)
#endif



#ifdef SGI
typedef unsigned char LOCK_TYPE;
typedef char          BARRIER_TYPE;

BARRIER_TYPE *MyInitBarrier();

#define MY_SLOCK(lp)          MyLock((lp))
#define MY_SUNLOCK(lp)        MyUnlock((lp))
#define MY_SINIT_LOCK(lp)     MyInitLock((lp))

#define MY_LOCK(lp)           MyLock((lp))
#define MY_UNLOCK(lp)         MyUnlock((lp))
#define MY_INIT_LOCK(lp)      MyInitLock((lp))

#define MY_LOCK_BACKUP(lp)    MyLock((lp))
#define MY_UNLOCK_BACKUP(lp)  MyUnlock((lp))

#define INIT_BARRIER(bp,limit) bp = MyInitBarrier()
#define WAIT_BARRIER(bp)       MyBarrier(bp,NumWorkers)
#endif

#if defined(POWER4)
typedef int LOCK_TYPE;
typedef int BARRIER_TYPE;

#define MY_SLOCK(lp)          p6k_pllock((lp))
#define MY_SUNLOCK(lp)        p6k_plfree((lp))
#define MY_SINIT_LOCK(lp)     p6k_lockasgn((lp))
#define MY_INIT_LOCK(lp)      p6k_lockasgn((lp))

#define MY_LOCK(lp)          p6k_pllock((lp))
#define MY_UNLOCK(lp)        p6k_plfree((lp))

#define MY_LOCK_BACKUP(lp)    p6k_pllock((lp))
#define MY_UNLOCK_BACKUP(lp)  p6k_plfree((lp))

#define INIT_BARRIER(bp,limit) p6k_barasgn(bp,&(limit))
#define WAIT_BARRIER(bp)       p6k_barrier(bp)

#define FLUSHALL p6k_flush_all()
#define FLUSHLINE(addr) p6k_flush_line((addr))
#define CACHESYNC dcs()
#define FLUSH(start,end) p6k_flush_region((start),(end))

#define FFL(addr) p6k_flush_line((addr))
#endif
