/*****************************************************************************\
 * File:	config.h
 * Description:	standard header files
 * Note:	Tools/osc.c and several files in Runtime (especially p-ppp.c)
 *		may need upgraded for a new code port.
\*****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
#include <sys/wait.h>

#ifdef SGI
#include <ulocks.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/sysmp.h>
#include <sys/schedctl.h>
#endif

#if defined(DJGPP)
#include <process.h>
#else
#include <sys/file.h>
#endif

extern double atof();
extern int atoi();

#if !defined(RS6000) && !defined(DJGPP) && !defined(SGI)
extern char *malloc();
extern int free();
#endif

#if !defined(DJGPP)
extern void exit();
#endif

#ifdef USE_TIMES
#  include <sys/types.h>
#  include <sys/times.h>
#  ifdef SGI
#    include <sys/param.h>
#  endif
#endif

#ifdef SUN
#  define CTIME_DEFINED
#  include <sys/time.h>
#  include <sys/resource.h>
#  define CLK_TCK 60
#  define const
#endif

#ifdef SGI
#  define CTIME_DEFINED
#  include <sys/time.h>
#  include <sys/resource.h>
#endif

#ifdef CRAY
#  define CTIME_DEFINED
#  include <time.h>
#  include <sys/time.h>
#endif

#if defined(HPUXPA) || defined(RS6000)
#  define CTIME_DEFINED
#  include <time.h>
#  include <sys/time.h>
#  include <sys/resource.h>
#endif

#ifndef CTIME_DEFINED
#  include <sys/time.h>
#  include <sys/resource.h>
#endif

#ifndef CLK_TCK
#  define CLK_TCK sysconf(_SC_CLK_TCK)
#endif

#define PRINTF		(void)printf
#define FPRINTF		(void)fprintf
#define SPRINTF		(void)sprintf
#define FCLOSE		(void)fclose
#define FPUTC		(void)fputc
#define UNGETC		(void)ungetc
#define SIZEOF(x)	((int)sizeof(x))

#ifdef NO_ASSERTIONS
#define ASSERT(w, x)	{}	/* w is assertion logical, x is text */
#define UNEXPECTED(x)	{}	/* x is unexpected text */
#define UNIMPLEMENTED(x) {}	/* x is unimplemented feature text */
#define ERRORINFO(y, z)	{}	/* y is error info format, z is value */
#else
#ifdef ERROR_ASSERTIONS
#define Warn1 Error1
#endif
#define ASSERT(w, x)	{if (!w) { char msg[1000]; SPRINTF(msg, \
			"assert <%s> failed in %s line %d", \
			x, __FILE__, __LINE__); Warning1( msg ); }}
#define UNEXPECTED(x)	{char msg[1000]; SPRINTF(msg, \
			"unexpected <%s> in %s line %d", \
			x, __FILE__, __LINE__); Warning1( msg );}
#define UNIMPLEMENTED(x) {char msg[1000]; SPRINTF(msg, \
			"unimplemented <%s> in %s line %d", \
			x, __FILE__, __LINE__); Warning1( msg );}
#define ERRORINFO(y, z)	{char msg[1000]; SPRINTF(msg, "  info "); \
			SPRINTF(&msg[7], y, z); Warning1( msg );}
#endif

#if !defined(TRUE)
#define TRUE             1
#define FALSE            0
#endif
