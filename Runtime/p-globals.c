#include "world.h"

int     NumWorkers       = DEFAULT_NUM_WORKERS;
int     DsaSize          = DEFAULT_DSA_SIZE;

int     BindParallelWork = FALSE;

int     XftThreshold     = DEFAULT_XFT_THRESHOLD;
int     LoopSlices       = -1;
int     GatherPerfInfo   = FALSE;
int     ArrayExpansion   = DEFAULT_ARRAY_EXPANSION;
int     NoFibreOutput    = FALSE;

int     UsingSdbx        = FALSE;

int     Sequential       = FALSE;

POINTER SisalMainArgs;

FILE   *FibreInFd  = stdin;
FILE   *FibreOutFd = stdout;
FILE   *PerfFd     = stderr;

char	DefaultLoopStyle = 'B';	/* Blocked loops is the default */


char	ArgumentString[1024] = "";
