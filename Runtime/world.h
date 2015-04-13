#ifndef _WORLD_LOADED
#define _WORLD_LOADED

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>

#ifdef SGI
#include <ulocks.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/sysmp.h>
#include <sys/schedctl.h>
#endif

#define SIZEOF(x)	((int)sizeof(x))
#define FPRINTF		(void)fprintf
#define SPRINTF		(void)sprintf
#define PRINTF		(void)printf
#define FCLOSE		(void)fclose
#define FPUTC		(void)fputc
#define UNGETC		(void)ungetc

#define TRUE             1
#define FALSE            0

#ifndef NULL
#define NULL             0
#endif

long atol();
void bcopy();
void ShutDownDsa();
void ParseTracerCommand();
void InitDsa();
void InitSignalSystem();

#ifdef CRAY
double   atof();
#endif

#include "conf.h"
#include "locks.h"
#include "process.h"
#include "fibre.h"
#include "arrays.h"
#include "interface.h"

#endif
