#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include "usage.h"		/* One-liner descr. of options */

#ifndef RS6000
extern char *malloc();
#endif

#ifndef MAX_PATH
#  define MAX_PATH	(1024)
#endif

#define TRUE     1
#define FALSE    0
#define ERROR    1
#define OK       0
#define SAME     0

/* ABSOLUTE DIRECTORY PATHS TO SISAL COMPILER EXECUTABLES */

#ifndef BIN_PATH
#define BIN_PATH   "/usr/local/bin"
#endif

#define StrCat( r, s1, s2 ) strcat( strcpy( (r), (s1) ), (s2) )
#define Warning(x,y )       fprintf( stderr, "OSC WARNING: %s %s\n", x, y )

#ifndef MAXARGS
#define MAXARGS  500
#endif

#define NAMESIZE 1000

/* ------------------------------------------------------------------------- */
#define SIS      1  /* file.sis  */            /* LEGAL FILE SUFFIX CODES */
#define IF1      4  /* file.if1  */
#define MONO     3  /* file.mono */
#define OPT      2  /* file.opt  */
#define MEM      5  /* file.mem  */
#define UP       6  /* file.up   */
#define PART     7  /* file.part */
#define COSTS    8  /* file.costs*/
#define C        9  /* file.c    */
#define S        10 /* file.s, file.o, or file.a */
#define O        11 /* file.o    */
#define A        12 /* file.a    */
#define F        13 /* file.f    */
#define I        14 /* file.i    */

#define LD       14

/* ------------------------------------------------------------------------- */
/* File names								     */
/* ------------------------------------------------------------------------- */
#ifndef USE_TMPDIR
#  define USE_TMPDIR ""
#endif
char	*TMPDIR = USE_TMPDIR;	/* Directory for temporaries */
char	*TMPPATH = "";		/* File head for temporaries */
char   *file;			/* FILE NAME FOR CURRENT COMPILATION PHASE */
char   *root;			/* ROOT OF file */
char   *costs = NULL;		/* COSTS file */
char   *cfile = "";		/* GENERATED C FILE */
char   *LoopReportIn = "-";	/* Name of Loop report to use */
char   *LoopReportOut = "-";	/* Name of Loop report to generate */


/* ------------------------------------------------------------------------- */
/* Command line items							     */
/* ------------------------------------------------------------------------- */
char   cmdline1[MAX_PATH*4];	/* Constructed command line for time */
char  *av [MAXARGS];		/* COMMAND LINE OF CURRENT COMPILATION PHASE */
int    avcnt   = -1;		/* INDEXES TO av, sis, if1, opt, ld, etc. */

/* ------------------------------------------------------------------------- */
/* File names and lists							     */
/* ------------------------------------------------------------------------- */
char  *sis [MAXARGS];     /* file.sis */
char  *sisi[MAXARGS];     /* file.sis -> file.i */
char  *sisif1[MAXARGS];     /* file.sis -> file.if1 */
int    siscnt  = -1;

char  *i   [MAXARGS];     /* file.i   */
char  *iif1[MAXARGS];     /* file.i -> file.if1  */
int    icnt    = -1;

char  *if1 [MAXARGS];     /* file.if1 */
int    if1cnt  = -1;

char  *f   [MAXARGS];     /* file.f */
int    fcnt    = -1;

char  *c   [MAXARGS];     /* file.c */
int    ccnt    = -1;

char  *ld [MAXARGS];      /* file.c, file.s, file.o, file.a */
int    ldcnt   = -1;

char  *opt[MAXARGS];      /* file.mono,file.if1,file.mem,file.up,file.part */
int    optcnt  = -1;

char   hybridOPTION[NAMESIZE];
char   hybridFILE[NAMESIZE];
char   hybridROOT[NAMESIZE];
char   hybridDOT[NAMESIZE];

/* ------------------------------------------------------------------------- */
/* Absolute paths to sisal executables and directories			     */
/* ------------------------------------------------------------------------- */
char   sisalpt  [NAMESIZE];	/* Parse tables */
char   sisal    [NAMESIZE];	/* frontend */
char   spprun   [NAMESIZE];
char   spp2     [NAMESIZE];
char   if1opt   [NAMESIZE];
char   if1ld    [NAMESIZE];
char   if2mem   [NAMESIZE];
char   if2up    [NAMESIZE];
char   if2part  [NAMESIZE];
char   if2gen   [NAMESIZE];
char  *ff	= "FF_HELP";	/* FORTRAN compiler */
char  *cc	= "CC_HELP";	/* C compiler */

char   incl     [NAMESIZE];	/* include file directory */
char   libs     [NAMESIZE];	/* library directory */
char   srt0     [NAMESIZE];	/* sisal runtime startup file */
char   buf1     [NAMESIZE];	/* cost file directory */

/* ------------------------------------------------------------------------- */
/* Compilation Phase Control */
/* ------------------------------------------------------------------------- */
int    start = -1;		/* WHICH PHASE TO START COMPILATION */
int    stopIF1    = FALSE;	/* CONTROL END PHASE */
int    stopCPP    = FALSE;
int    stopMONO   = FALSE;
int    stopOPT    = FALSE;
int    stopDI     = FALSE;
int    stopMEM    = FALSE;
int    stopUP     = FALSE;
int    stopPART   = FALSE;
int    stopC      = FALSE;
int    stopS      = FALSE;
int    stopc      = FALSE;

/* ------------------------------------------------------------------------- */
/* Support for dynamic patch facility					     */
/* ------------------------------------------------------------------------- */
char   *Patches [1];		/* Dynamic patches to apply */
int    PatchCount = 0;		/* Number of active patches */

/* ------------------------------------------------------------------------- */
/* OSC tool options							     */
/* ------------------------------------------------------------------------- */
int    cppoptioncnt = -1;	/* Options for CPP */
char   *cppoptions[MAXARGS];

int    ffoptionscnt = -1;	/* Options for FORTRAN */
char   *ffoptions[MAXARGS];

int    ccoptionscnt = -1;	/* Options for C */
char   *ccoptions[MAXARGS];

/* ------------------------------------------------------------------------- */
/* Timing, Tracing, Profiling, etc... (function lists)			     */
/* ------------------------------------------------------------------------- */
int    flopoptioncnt = -1;	/* Get floating point op counts */
char   *flopoptions[MAXARGS];

int    timeall = FALSE;		/* Get timing information */
int    timeoptioncnt = -1;
char   *timeoptions[MAXARGS];

int    traceall = FALSE;	/* Create tracing code */
int    traceoptioncnt = -1;
char   *traceoptions[MAXARGS];

int    calloptioncnt = -1;	/* Never inline */
char   *calloptions[MAXARGS];

int    coptioncnt = -1;		/* C interface */
char   *coptions[MAXARGS];

int    eoptioncnt = -1;		/* Entry point */
char   *eoptions[MAXARGS];

int    foptioncnt = -1;		/* FORTRAN interface */
char   *foptions[MAXARGS];

/* ------------------------------------------------------------------------- */
/* Option control for backend						     */
/* ------------------------------------------------------------------------- */
/* How to run parallel loops */
char	LoopStyle[4]	= "-SR";

/* Generate cross listing */
int	list		= FALSE;

/* Use a pre-processor? */
#if USESPP
int	nocpp		= FALSE;
#else
int	nocpp	 	= TRUE;
#endif

/* Override default choices for C and FORTRAN compilers */
int	newcc		= FALSE;
int	newff		= FALSE;

/* Allow FORTRAN instrinsic bit functions in SISAL code */
int	intrinsics	= TRUE;


int	smodule		= FALSE;

int	nobrec		= FALSE;

int	prof		= FALSE;
int	Prof		= FALSE;

int	avector		= FALSE;
int	AggressiveVector = FALSE;
int	cvector		= FALSE;

int	useORTS		= TRUE;

int	forC		= FALSE;
int	forF		= FALSE;
int	BindProcessors	= FALSE;

int	dead		= TRUE;

int	debug		= FALSE;
int	DeBuG		= FALSE;

int	freeall		= FALSE;

int	verbose		= FALSE;

int	Warnings	= TRUE;	/* Display warning messages */
int	bounds		= TRUE;	/* Do bounds checking by default */
int	SISglue		= FALSE;
int	sdbx		= FALSE;


char	*unroll		= "-U2";

int	noinvert	= FALSE;
int	nostrip		= FALSE;
int	nosplit		= FALSE;
int	nodope		= FALSE;
int	noamove		= FALSE;

char	*preb		= "-Y3";


int	norag		= FALSE;
int	nobip		= FALSE;
int	bipmv		= TRUE;

int	hybrid		= FALSE;
int	dohybrid	= FALSE;

int	noopt		= FALSE;
int	noimp		= TRUE;
int	noaimp		= FALSE;

int	nltss		= FALSE;

#ifdef ALLIANT
int	alliantfx	= TRUE;
int	cRay		= FALSE;
#else
#  ifdef CRAY
int	alliantfx	= FALSE;
int	cRay		= TRUE;
#  else
int	alliantfx	= FALSE;
int	cRay		= FALSE;
#  endif
#endif

#ifdef ALLIANT
int	novec		= FALSE;
#else
#  ifdef CRAY
int	novec		= FALSE;
#  else
int	novec		= TRUE;
#  endif
#endif

int	noif1opt	= FALSE;
int	noif2mem	= FALSE;
int	noif2up		= FALSE;

int	noinline	= FALSE;
int	noscalar	= FALSE;
int	norecf		= FALSE;


int	noOinvar	= FALSE;
int	noinvar		= FALSE;
int	nocse		= FALSE;

int	nonormidx	= FALSE;

#ifdef CRAY
int	newchains	= TRUE;
int	movereads	= TRUE;
int	chains		= TRUE;
#else
int	movereads	= FALSE;
int	newchains	= FALSE;
int	chains		= FALSE;
#endif

int	notgcse		= TRUE;

int	noifuse		= FALSE;
int	nodfuse		= FALSE;
int	nosfuse		= FALSE;
int	nogcse		= FALSE;
int	noaggressive	= TRUE;
int	nofold		= FALSE;
int	inter		= FALSE;
int	inlineall	= FALSE;

int	explode		= FALSE;
int	explodeI	= FALSE;

int	prog		= FALSE;

#ifdef ENCORE
int	nocagg		= TRUE;
int	nogshared	= TRUE; 
#else
int	nocagg		= FALSE;
int	nogshared	= FALSE; 
#endif

int	nomig		= FALSE;
int	nocom		= FALSE;
int	nostr		= FALSE;

int	dbl		= FALSE;
int	flt		= FALSE;
int	noregs		= FALSE;

int	nofcopy		= FALSE;
int	noassoc		= FALSE;

/* ------------------------------------------------------------ */
/* Generate parallel code? */
#if SUNIX || SUN || RS6000
int	concur		= FALSE;
#else
int	concur		= TRUE;
#endif

/* ------------------------------------------------------------ */
/* Parallel loop depth */
char	*level		= NULL;

/* ------------------------------------------------------------ */
/* Inner loop parallelization level */
char	*atlevel	= NULL;

/* ------------------------------------------------------------ */
/* Number of processors to consider */
char	*procs		= NULL;

/* ------------------------------------------------------------ */
/* Smallest loop to slice (if2part) */
char	*huge		= NULL;

/* ------------------------------------------------------------ */
/* Assumed iteration count for partitioner */
char	*iter		= NULL;

/* ------------------------------------------------------------ */
/* Name of absolute to produce */
char	*aabs		= NULL;

/* ------------------------------------------------------------ */
/* Check safety of this name (sisal extensions) */
int	NameSafety	= TRUE;

/* ------------------------------------------------------------ */
/* Module database name */
char	*mdb		= NULL;

/* ------------------------------------------------------------ */
/* General information */
int	info		= FALSE;

/* ------------------------------------------------------------ */
/* Vector information */
int	vinfo		= FALSE;

/* ------------------------------------------------------------ */
/* Concurrency information */
int	cinfo		= FALSE;

/* ------------------------------------------------------------ */
/* Copy information */
int	cpyinfo		= FALSE;

/* ------------------------------------------------------------ */
/* Flop count information*/
int	flpinfo		= FALSE;

/* ------------------------------------------------------------ */
/* Show SISAL source lines in generated C code */
int	ShowSource	= FALSE;

/* ------------------------------------------------------------ */
/* Allow the Min. Slice size to throttle concurrent execution. */
int	MinSliceThrottle = FALSE;

/* ------------------------------------------------------------ */
/* Do not keep intermediate files */
int	Keep		= FALSE;

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
#define TEMPSIZE (1000)
static char *TempFile[TEMPSIZE];
static int TempCount=0;
static void RegisterTempFile(s)
     char   	*s;
{
  if ( TempCount >= TEMPSIZE ) return;
  TempFile[TempCount++] = s;
}

/**************************************************************************/
/* LOCAL  **************          DeathWish        ************************/
/**************************************************************************/
/* PURPOSE:  If the compiler dies unexpectedly, perform some cleanup work */
/**************************************************************************/
static void DeathWish(sig,code,scp)
     int	sig,code;
{
  int		i;
  static void RemoveFile();
  static int  GotHere = 0;

  if ( GotHere ) exit(1);	/* Just quit if reentry is possible */
  GotHere = 1;

  fprintf(stderr,"*** Killed %d[%d] ***\n",sig,code);
  for(i=0;i<TempCount;i++) RemoveFile(TempFile[i]);
  exit(1);
}

/**************************************************************************/
/* Global **************          ParseMap         ************************/
/**************************************************************************/
/* PURPOSE:  Turns strings of the form 1-3,15-16 into 16-bit bit masks.	  */
/*	     For instance, ParseMap("1-3,15-16") will return 0xC007.  Bit */
/*	     positions are numbered from 1 to 16 (low to high) and out of */
/*	     range positions are ignored.				  */
/*									  */
/*	     Stolen from Matt Hayne's partitioner code			  */
/**************************************************************************/

unsigned int ParseMap(string)
     char *string;
{
  unsigned int map = 0;		/* The initial map */
  int anInt;			/* tmp counter */
  char *index;			/* index into string */
  int start, stop;		/* tmp vars */
#define AddToMask(map,val) if(val>0 && val <17) map |= (1<<val-1)

  map = 0;

  if (string == NULL) {		/* no string, return this */
    return map;
  }				/* if */

  start = -1;
  for (index = string ; *index ; ) { /* run down string */
    switch(*index) {
     case '0': case '1': case '2': case '3': case '4': 
     case '5': case '6': case '7': case '8': case '9':
      if (start == -1) {	/* if we ar looking for a start position */
	start = atoi(index);
	while(isdigit(*index)) index++;
      } else {			/* have start, looking for stop */
	stop = atoi(index);
	if (start < stop) {
	  for (anInt = start ; anInt <= stop ; anInt++) {
	    AddToMask(map,anInt);
	  }			/* for */
	} else {		/* stop <= start */
	  for (anInt = start ; anInt >= stop ; anInt--) {
	    AddToMask(map,anInt);
	  }			/* for */
	}			/* if */

	while(isdigit(*index)) index++;
	start = stop = -1;
      }				/* if */

      break;

     case ',':
     case ' ':
     case 0:			/* for end of string */
      if (start != -1) {
	AddToMask(map,start);
	start = -1;
      }				/* if */

      index++;
      break;

     case '-':
      if (start == -1) {	/* error, range must have a start */
	fprintf(stderr, "range with no start");
	exit(1);
      }				/* if */

      index++;
      break;

     default:
      fprintf(stderr, "error unknown character <%c>\n", *index);
      exit(1);
    }				/* switch */
  }				/* for */

  /*
   * if we make it here with start equal something, we have hit the end
   * of the string, and have to grab the last item.
   */
  if (start != -1) {
    AddToMask(map,start);
  }				/* if */

  return map;

#undef AddToMask
}				/* ParseMap */

/**************************************************************************/
/* LOCAL  **************           Error2          ************************/
/**************************************************************************/
/* PURPOSE: PRINT m1 AND m2 TO stderr AND ABORT EXECUTION; RETURNING AN   */
/*          ERROR STATUS OF 1.                                            */
/**************************************************************************/

static void Error2( m1, m2 )
char *m1;
char *m2;
{
    fprintf( stderr, "OSC ERROR: %s %s\n", m1, m2 );
    exit( ERROR );
}


/**************************************************************************/
/* LOCAL  **************       GetFileSuffix       ************************/
/**************************************************************************/
/* PURPOSE: RETURN A POINTER TO THE SUFFIX OF FILE nm.  IF A SUFFIX DOES  */
/*          NOT EXIST, AN ERROR MESSAGE IS PRINTED.  A LEGAL SUFFIX       */
/*          BEGINS WITH A ".", DOES NOT CONTAIN WHITE CHARACTERS, PERIODS,*/
/*          OR BACKSLASHES, AND TERMINATES WITH A NULL CHARACTER.         */
/**************************************************************************/

static char *GetFileSuffix( nm )
char *nm;
{
    register char *p;
    register char *s;
    register char  NameChar;

    for ( p = nm, s = NULL; NameChar = *p; p++ )
        switch ( NameChar ) {
            case '/': 
                s = NULL;
                break;

            case '.':
                s = p;
                break;

            default:
                break;
            }

    return( s );
}


/**************************************************************************/
/* LOCAL  **************           Submit          ************************/
/**************************************************************************/
/* PURPOSE: SUBMIT COMMAND cmd TO UNIX USING av AS THE COMMAND LINE.      */
/*          A CHILD PROCESS IS CREATED USING fork THAT DOES AN execvp     */
/*          TO DO cmd. THE PARENT PROCESS WAITS FOR THE CHILD TO COMPLETE,*/
/*          RETURNING ERROR IF THE CHILD'S TERMINATION STATUS IS NOT EQUAL*/
/*          TO 0. IN DEBUG OR VERBOSE MODE THE ARGUMENT LIST TO THE       */
/*          SUBMITTED COMMAND IS PRINTED TO stderr. THE COMMAND IS ONLY   */
/*          SUBMITTED IF debug ISN'T ENABLED.                             */
/**************************************************************************/

static int Submit( cmd )
char *cmd;
{
    register int idx;
             int s;
    register int id;

    if ( Prof || prof ) {
      strcpy(cmdline1, "time " );
      strcat(cmdline1, cmd);
      for (idx = 1; av[idx] != NULL; idx++) {
	strcat(cmdline1," ");
	strcat(cmdline1,av[idx]);
        }

      if ( debug || verbose )
	fprintf(stderr, "\n*** %s ***\n",cmdline1);

      if ( debug )
	  return ( OK );

      s = system(cmdline1);

      if (s > 0)
	return ( ERROR );

      return( OK );
      }

    if ( debug || verbose ) {
        for ( idx = 0; av[idx] != NULL; idx++) {
	    if ( DeBuG && idx == 0 ) {
              fprintf( stderr, "%s ", cmd );
	      continue;
	      }

            fprintf( stderr, "%s ", av[idx] );
	    }

        fprintf( stderr, "\n" );
        }

    if ( debug )
        return( OK );

    id = fork();

    if ( id == -1)
        Error2( "submit failed:", "no more processes allowed" );

    if ( id == 0 ) {
        execvp( cmd, av );
        Error2( "Can't find Command:", cmd );
        }

    while ( id != wait( &s ) ) ;

    if ( s > 0 ) /* CANN */
        return( ERROR );

    return( OK );
}


/**************************************************************************/
/* LOCAL  **************       SetFileSuffix       ************************/
/**************************************************************************/
/* PURPOSE: RETURN A POINTER TO THE FILE NAME WHOSE ROOT IS r AND SUFFIX  */
/*          IS s.                                                         */
/**************************************************************************/

static char *SetFileSuffix( prefix, r, s )
char *prefix;
char *r;
char *s;
{
    register char *nm;
    static   int  unique= 0;

    nm = (char *) malloc( sizeof(char) * NAMESIZE );

    if ( *prefix ) {
      unique++;
      sprintf( nm, "%s_%d%s", prefix, unique, s );
    } else {
      sprintf( nm, "%s%s", r, s );
    }

    return( nm );
}


/**************************************************************************/
/* LOCAL  **************        GetFileType        ************************/
/**************************************************************************/
/* PURPOSE: RETURN A CODE IDENTIFYING THE SUFFIX OF FILE nm. ENCOUNTERING */
/*          AN ILLEGAL SUFFIX RESULTS IN AN ERROR MESSAGE.                */
/**************************************************************************/

static int GetFileType( nm )
char *nm;
{
    register char *s;

    s = GetFileSuffix( nm );
    if ( !s ) return NULL;

    if ( strcmp( s, ".sis"  ) == SAME ) return(SIS);
    if ( strcmp( s, ".if1"  ) == SAME ) return(IF1);
    if ( strcmp( s, ".mono" ) == SAME ) return(MONO);
    if ( strcmp( s, ".opt"  ) == SAME ) return(OPT);
    if ( strcmp( s, ".mem"  ) == SAME ) return(MEM);
    if ( strcmp( s, ".up"   ) == SAME ) return(UP);
    if ( strcmp( s, ".part" ) == SAME ) return(PART);
    if ( strcmp( s, ".costs") == SAME ) return(COSTS);
    if ( strcmp( s, ".a")     == SAME ) return(A);
    if ( strcmp( s, ".c")     == SAME ) return(C);
    if ( strcmp( s, ".s")     == SAME ) return(S);
    if ( strcmp( s, ".o")     == SAME ) return(O);
    if ( strcmp( s, ".f")     == SAME ) return(F);
    if ( strcmp( s, ".i")     == SAME ) return(I);

    return (int)NULL;
}


/**************************************************************************/
/* LOCAL  **************         RemoveFile        ************************/
/**************************************************************************/
/* PURPOSE: UNLINK FILE nm FROM THE CURRENT WORKING DIRECTORY.            */
/**************************************************************************/

static void RemoveFile( nm )
char *nm;
{
  if ( Keep ) return;

  if ( GetFileType( nm ) == start ) return;

  if ( debug || verbose ) fprintf( stderr, "unlink %s\n", nm );

  if ( !debug ) unlink( nm );
}


/**************************************************************************/
/* LOCAL  **************        GetFileRoot        ************************/
/**************************************************************************/
/* PURPOSE: RETURN A COPY OF FILE nm'S ROOT.                              */
/**************************************************************************/

static char *GetFileRoot( nm )
char *nm;
{
    register char *s;
    register char *r;
    register char *p;

    strcpy( r = (char *) malloc( sizeof(char) * NAMESIZE ), nm );

    s  = GetFileSuffix( r );

    if ( s == NULL ) Error2( "illegal file name:", nm );

    *s = '\0';

    /* Go back and find the begining of the name... */
    for(p=s; p>r; p--) if ( *p == '/' ) {p++; break;}

    return( p );
}


/**************************************************************************/
/* LOCAL  **************      LongGetFileRoot      ************************/
/**************************************************************************/
/* PURPOSE: RETURN A COPY OF FILE nm'S ROOT (with pathname info)          */
/**************************************************************************/

static char *LongGetFileRoot( nm )
char *nm;
{
    register char *s;
    register char *r;

    strcpy( r = (char *) malloc( sizeof(char) * NAMESIZE ), nm );

    s  = GetFileSuffix( r );

    if ( s == NULL ) Error2( "illegal file name:", nm );

    *s = '\0';

    return( r );
}

/**************************************************************************/
/* LOCAL  **************        SubmitNQS          ************************/
/**************************************************************************/
/* PURPOSE:  Resubmit this job as an NQS batch job			  */
/**************************************************************************/
static void SubmitNQS(argc,argv,nqs_idx)
     int	argc,nqs_idx;
     char	*argv[];
{
  int		i,avcnt;
  char		*p;
  char		scriptname[100];
  FILE		*script;
  int		status;

  /* ------------------------------------------------------------ */
  /* Open a temp file for the script				  */
  /* ------------------------------------------------------------ */
  sprintf(scriptname,"#NQS%d",getpid());
  script = fopen(scriptname,"w");

  for(i=0;i<nqs_idx;i++) {
    for(p = argv[i]; *p; p++) {
      switch(*p) {
      case '\'':	fputs("\"'\"",script); break;
      case '!':		fputs("\"\\!\"",script); break;
	
      case ' ': case '\t': case '$': case '(': case ')': case ';':
      case '<': case '>':  case '?': case '`': case '{': case '}':
      case '\\': case '&': case '|': case '~':
	fprintf(script,"'%c'",*p); break;
      default:
	fputc(*p,script);
      }
    }
    fputc(' ',script);
  }
  fputc('\n',script);
  fprintf(script,"/bin/rm -f '%s'\n",scriptname);
  fclose(script);
  RegisterTempFile(scriptname);

  /* ------------------------------------------------------------ */
  /* Build up the queue command...				  */
  /* ------------------------------------------------------------ */
  avcnt = 0;
  av[avcnt++] = "qsub";
  av[avcnt++] = scriptname;
  for(i=nqs_idx+1; i<argc; i++) av[avcnt++] = argv[i];
  av[avcnt] = (char*)NULL;

  /* ------------------------------------------------------------ */
  /* ...and submit it for execution				  */
  /* ------------------------------------------------------------ */
  status = Submit(av[0]);
  if ( status != OK ) fprintf(stderr,"%s %s failed\n",av[0],scriptname);

  RemoveFile(scriptname);
  exit(status);
}

/**************************************************************************/
/* LOCAL  **************      ParseCommandLine     ************************/
/**************************************************************************/
/* PURPOSE: PARSE THE OSC COMMAND LINE GATHERING FILE NAMES INTO THEIR    */
/*          APPROPRIATE GROUPS AND ENABLING COMPILATION OPTIONS.  OPTION  */
/*          AND FILE NAME AMBIGUITIES RESULT IN ERROR MESSAGES.           */
/**************************************************************************/

static void ParseCommandLine( argc, argv )
int    argc;
char **argv;
{
  register int idx;
  char	       *CorrectUsage;	/* Explanation for option error */

  for ( idx = 1; idx < argc; idx++ ) {
    CorrectUsage = NULL;

    if ( *argv[idx] != '-') {
      switch ( GetFileType( argv[idx] ) ) {
       case (int)NULL:
	Error2( "illegal file name:", argv[idx] );
	break;

       case I:
	i[++icnt] = argv[idx];
	break;

       case SIS:
	sis[++siscnt] = argv[idx];
	break;

       case IF1:
	if1[++if1cnt] = argv[idx];
	break;

       case OPT:
       case MONO:
       case MEM:
       case UP:
       case PART:
	opt[++optcnt] = argv[idx];
	break;

       case C:
	c[++ccnt] = argv[idx];
	break;

       case F:
puts(argv[idx]);
	f[++fcnt] = argv[idx];
	break;

       case COSTS:
	costs = argv[idx];
	break;

       case A:
       case O:
       case S:
	ld[++ldcnt] = argv[idx];
	break;

       default:
	goto OptionError;
      }

      continue;
    }

    /* ------------------------------------------------------------ */
    /* Do not change options.h directly.  Modify the options	    */
    /* file and run ``make newoptions''.  This will update the	    */
    /* osc.m man page file and update options.h and usage.h	    */
    /* This makefile target requires the parseopts tool		    */
    /* (available separately).					    */
    /* ------------------------------------------------------------ */
#include "options.h"

  }

  /* ------------------------------------------------------------ */
  /* Compute the TMPPATH from the TMPDIR			  */
  /* ------------------------------------------------------------ */
  if ( *TMPDIR ) {
    TMPPATH = (char*)(malloc(strlen(TMPDIR)+20));
    (void)sprintf(TMPPATH,"%s/tmp%d",TMPDIR,getpid());
  } else {
    TMPPATH = "";
  }

  /* ------------------------------------------------------------ */
  /* Get the intermediate names for the .sis and .i files	  */
  /* ------------------------------------------------------------ */
  for( idx=0; idx <= siscnt; idx++ ) {
    sisi[idx] = SetFileSuffix((stopCPP)?(""):(TMPPATH),
			      GetFileRoot(sis[idx]), ".i" );
    sisif1[idx] = SetFileSuffix((stopIF1)?(""):(TMPPATH),
				GetFileRoot(sis[idx]), ".if1" );
  }
  for( idx=0; idx <= icnt; idx++ ) {
    iif1[idx] = SetFileSuffix((stopIF1)?(""):(TMPPATH),
			      GetFileRoot(i[idx]), ".if1" );
  }

  /* ------------------------------------------------------------ */
  /* Make sure that we aren't overwriting a source file		  */
  /* ------------------------------------------------------------ */
  if ( NameSafety && aabs && GetFileType( aabs ) ) {
    Error2("Possible overwrite of source file.  You must use -oo",aabs);
  }

  /* if ( novec && !noaggressive )
    Warning( "-aggressive given with -novector, so ignored", "" ); */

  if ( noopt && explode ) {
    Warning( "-noopt disables -explode and -explodeI", "" );
    explode  = FALSE;
    explodeI = FALSE;
    }

  if ( inlineall && (inter || noinline || noopt) )
    Warning( "-inlineall conflicts with -inter, -noinline, and -noopt", "" );

  if ( alliantfx && cRay )
    Error2( "-alliantfx and -cray conflict", "" );

#ifdef CRAY
  /* if ( dbl ) */
    /* Warning( "-double_real ignored, -real assumed", "" ); */

  /* flt = TRUE; */
  /* dbl = FALSE; */
#endif

  if ( timeoptioncnt > -1 && (inlineall || inter) )
    Error2( "-time and -inlineall or -inter conflict", "" );

  if ( traceoptioncnt > -1 && (inlineall || inter) ) {
    Error2( "-trace and -inlineall or -inter conflict", "" );
  }

  if ( forC && forF )
    Error2( "-forC and -forFORTRAN conflict", "" );

  if ( (forC || forF) && smodule )
    Error2( "-forC and -forFORTRAN conflict with -module", "" );

  if ( !(forC || forF || smodule) )
   if ( eoptioncnt > 0 )
    Error2( "only one entry point allowed in a stand-alone program module", "");
      
  if ( cpyinfo )
    nocagg = TRUE;

  if ( bounds && sdbx )
    Error2( "-bounds and -sdbx conflict", "" );

  if ( BindProcessors && freeall )
    Error2( "-bind and -freeall conflict", "" );

  if ( flt && dbl )
    Error2( "-double_real and -real conflict", "" );

  if ( sdbx || bounds ) {
    novec = TRUE;
  }

  if ( sdbx ) {
    concur   = FALSE;
    noif2up  = TRUE;
    noif2mem = TRUE;
    noimp    = TRUE;
    SISglue = TRUE;
    inlineall = FALSE;
  }


  if ( noopt ) {
    noscalar = TRUE;
    noinline = TRUE;
    noif2mem = TRUE;
    noif2up  = TRUE;
    novec    = TRUE;
    avector  = FALSE;
    cvector  = FALSE;
    concur   = FALSE;
    preb   = "-Y0";
    nobip    = TRUE;
    explode = FALSE;
  } else {
    if ( nocse )
      noifuse = nodfuse = nosfuse = TRUE;
    }

  if ( !concur )
    level = "-L0";

  return;

OptionError:
  if ( CorrectUsage ) fprintf(stderr,"%s\n",CorrectUsage);
  Error2( "illegal option:", (argv[idx])?(argv[idx]):"" );
}

/**************************************************************************/
/* LOCAL  **************   SubmitFrontendRequest   ************************/
/**************************************************************************/
/* PURPOSE: SUBMIT A REQUEST TO THE FRONTEND TO COMPILE file.             */
/**************************************************************************/

static int SubmitFrontendRequest( SISfile, IF1file, farg )
char *SISfile;
char *IF1file;
char *farg;
{
  char fopt[MAX_PATH+3];

#define AddAV(s) {char *_p,*_f; int _n; for(_f=(s),_p=_f;*_p;) {\
      if (*_p == ' ' || *_p == '\t') {\
	_n=_p-_f; av[avcnt] = (char*)(malloc(_n+1));\
	strncpy(av[avcnt],_f,_n+1); av[avcnt][_n] = NULL; avcnt++;\
	while(*_p == ' ' || *_p == '\t') {_p++;} _f = _p;\
      } else {_p++;}} if ( *_f) {av[avcnt++] = _f;}\
  }
  avcnt = 0;
  AddAV(sisal);
  AddAV("-dir");
  AddAV(sisalpt);
  AddAV(SISfile);
  AddAV("-O");
  AddAV(IF1file);

  sprintf( fopt, "-F%s", farg );
  AddAV(fopt);
    
  if ( list ) AddAV("-xref");

  if ( SISglue ) AddAV("-SISglue");

  av[avcnt] = NULL;

  return( Submit( sisal ) );
}


/**************************************************************************/
/* LOCAL  **************       SubmitCommands      ************************/
/**************************************************************************/
/* PURPOSE: DETERMINE THE COMPILATION STARTING POINT, SUBMIT COMPILATION  */
/*          PHASES TO UNIX, AND RETURN THE TERMINATION STATUS.            */
/**************************************************************************/

static int SubmitCommands()
{
  register int   idx, jdx, cnt;
  register char *tmp0, *tmp1, *tmp2, *tmp3, *tmp4;
  char  module[200];
  char		dashi[16];
  char		dashI[16];
  char		dashV[16];
  int		pat;
  char		*filetail;
  int		status;

  if ( icnt >= 0 || siscnt >= 0 ) {
    if ( optcnt >= 0 ) Error2( "USAGE:", "conflicting file types" );
    
    if ( siscnt < 0 ) {
      root = GetFileRoot( i[0] );
    } else {
      root = GetFileRoot( sis[0] );
    }

    file  = SetFileSuffix((stopMONO)?(""):(TMPPATH),root, ".mono" );
    start = SIS;

  } else if ( if1cnt >= 0 ) {
    if ( optcnt >= 0 ) Error2( "USAGE:", "conflicting file types" );
    
    root  = GetFileRoot( if1[0] );
    file  = SetFileSuffix((stopMONO)?(""):(TMPPATH),root, ".mono" );
    start = IF1;

  } else if ( optcnt >= 0 ) {
    if ( (optcnt > 0) ) Error2( "USAGE:", "conflicting file types" );

    root  = GetFileRoot( opt[0] );
    file  = opt[0];
    start = GetFileType( file );

  } else if ( ccnt >= 0 ) {
    root  = NULL;
    file  = NULL;
    start = C;

  } else if ( ldcnt >= 0 ) {
    root  = NULL;
    file  = NULL;
    start = LD;

  } else if ( fcnt >= 0 ) {
    root  = NULL;
    file  = NULL;
    start = F;

  } else {
    return( OK );
  }

  if ( root != NULL ) {
    tmp0 = SetFileSuffix((stopOPT)?(""):(TMPPATH),root,  ".opt"  );
    tmp1 = SetFileSuffix((stopMEM)?(""):(TMPPATH),root,	 ".mem"  );
    tmp2 = SetFileSuffix((stopUP)?(""):(TMPPATH),root,	 ".up"   );
    tmp3 = SetFileSuffix((stopPART)?(""):(TMPPATH),root, ".part" );
    tmp4 = SetFileSuffix((stopC)?(""):(TMPPATH),root,	 ".c"    );
  }

  switch ( start ) {
    /*******************************************************************/
  case SIS:
    for ( idx = 0; idx <= siscnt; idx++ ) {
      if ( !nocpp ) {
	avcnt = 0;
	AddAV(cc);
	sprintf( cmdline1, "%s%s %s '%s' %s %s",
		(Prof || prof)? "time " : "", spprun, spp2, av[0], 
		sis[idx],sisi[idx]);

	for ( cnt = 0; cnt <= cppoptioncnt; cnt++ ) {
	  strcat( cmdline1, " " );
	  strcat( cmdline1, cppoptions[cnt] );
	}

	if ( Prof || prof ) {
	  fprintf( stderr, "\n*** %s ***\n", cmdline1 );
	} else if ( debug || verbose ) {
	  fprintf( stderr, "%s\n", cmdline1 );
	}

	/* ------------------------------------------------------------ */
	/* About to create tempfile, so register it with the list	*/
	/* ------------------------------------------------------------ */
	RegisterTempFile(sisi[idx]);

	if ( !debug && system(cmdline1) != 0 ) {
	  RemoveFile( sisi[idx] );
	  return( ERROR );
	}
      }

      if ( stopCPP ) continue;

      /* ------------------------------------------------------------ */
      /* Make sure that dead frontend requests do not leave dead .i   */
      /* files laying around (Srdjan)				      */
      /* ------------------------------------------------------------ */
      RegisterTempFile(sisif1[idx]);
      if ( !nocpp ) {
	status = SubmitFrontendRequest(sisi[idx],sisif1[idx],sis[idx]);
      } else {
	status = SubmitFrontendRequest(sis[idx],sisif1[idx],sis[idx]);
      }

      /* ------------------------------------------------------------ */
      /* CLEAN UP temporary file.i FILES			      */
      /* ------------------------------------------------------------ */
      if ( !nocpp ) RemoveFile( sisi[idx] );

      if ( status != OK ) {
	for(jdx=0;jdx<=idx;jdx++) RemoveFile(sisif1[jdx]);
	return ERROR;
      }
    }

    if ( stopCPP ) return( OK );

    /* ------------------------------------------------------------ */
    /* Now compile any supplied .i files			    */
    /* ------------------------------------------------------------ */
    for ( idx = 0; idx <= icnt; idx++ ) {
      RegisterTempFile(iif1[idx]);
      status = SubmitFrontendRequest( i[idx], iif1[idx], i[idx] );
      if ( status != OK ) {
	for(jdx=0;jdx<=idx;jdx++) RemoveFile(iif1[jdx]);
	return( ERROR );
      }
    }
    /*******************************************************************/
  case IF1:
    if ( stopCPP || stopIF1 ) return( OK );

    av[0] = if1ld;
    av[1] = "-o";
    av[2] = file;
    RegisterTempFile(file);

    avcnt = 3;

    for(pat=0; pat<PatchCount; pat++) av[avcnt++] = Patches[pat];

    if ( smodule ) av[avcnt++] = "-S";

    if ( !Warnings ) av[avcnt++] = "-w";
    if ( Prof ) av[avcnt++] = "-W";
    if ( forF ) av[avcnt++] = "-F";
    if ( forC ) av[avcnt++] = "-C";


    for ( idx = 0; idx <= eoptioncnt; idx++ ) {
      av[avcnt++] = "-e";
      av[avcnt++] = eoptions[idx];
    }

    for ( idx = 0; idx <= coptioncnt; idx++ ) {
      av[avcnt++] = "-c";
      av[avcnt++] = coptions[idx];
    }

    for ( idx = 0; idx <= foptioncnt; idx++ ) {
      av[avcnt++] = "-f";
      av[avcnt++] = foptions[idx];
    }

    if ( cRay ) {
      av[avcnt++] = "-FU";
    } else {
#ifdef RS6000
      av[avcnt++] = "-FUN";
#else
      av[avcnt++] = "-FUR";
#endif
    }

    /* ------------------------------------------------------------ */
    /* Submit with if1 files from .sis files, .i files, and	      */
    /*.if1 files.						      */
    /* ------------------------------------------------------------ */
    for ( idx = 0; idx <= siscnt; idx++ ) av[avcnt++] = sisif1[idx];
    for ( idx = 0; idx <= icnt;   idx++ ) av[avcnt++] = iif1[idx];
    for ( idx = 0; idx <= if1cnt; idx++ ) av[avcnt++] = if1[idx];

    av[avcnt] = NULL;

    /* ------------------------------------------------------------ */
    /* Launch the monolith loader.				    */
    /* ------------------------------------------------------------ */
    status = Submit(if1ld);

    /* ------------------------------------------------------------ */
    /* Remove the .if1 files					      */
    /* ------------------------------------------------------------ */
    for ( idx = 0; idx <= siscnt; idx++ ) RemoveFile(sisif1[idx]);
    for ( idx = 0; idx <= icnt;   idx++ ) RemoveFile(iif1[idx]);
    for ( idx = 0; idx <= if1cnt; idx++ ) RemoveFile(if1[idx]);

    if ( status != OK ) {
      RemoveFile(file);
      return ERROR;
    }

    /*******************************************************************/
  case MONO:
    if ( stopMONO )
      return( OK );

    av[0] = if1opt; 
    av[1] = file;
    av[2] = tmp0;
    RegisterTempFile(tmp0);

    avcnt = 3;

    for(pat=0; pat<PatchCount; pat++) av[avcnt++] = Patches[pat];

    for ( idx = 0; idx <= calloptioncnt; idx++ ) {
      av[avcnt++] = "-#";
      av[avcnt++] = calloptions[idx];
    }

    if ( !Warnings ) av[avcnt++] = "-w";
    if ( Prof ) av[avcnt++] = "-W";

    if ( !dead ) av[avcnt++] = "-d";
    if ( SISglue ) av[avcnt++] = "-8";

    if ( sdbx ) av[avcnt++] = "-3";
    if ( noassoc ) av[avcnt++] = "-Y";

    if ( !noaggressive ) av[avcnt++] = "-G";

    if ( AggressiveVector ) av[avcnt++] = "-AggV";

    if ( nostrip ) av[avcnt++] = "-6";

    if ( concur ) av[avcnt++] = "-R";

    if ( noinline ) av[avcnt++] = "-x";

    if ( nonormidx ) av[avcnt++] = "-N";

    if ( cvector ) {
      if ( !novec )  av[avcnt++] = "-X";
      av[avcnt++] = "-C5";
    }
#ifdef CRAY
    if ( !novec )  av[avcnt++] = "-X";
    av[avcnt++] = "-C5";
#endif

    if ( avector ) {
      if ( !novec )  av[avcnt++] = "-X";
      av[avcnt++] = "-A";
    }
#ifdef ALLIANT
    if ( !novec )  av[avcnt++] = "-X";
    av[avcnt++] = "-A";
#endif

    if ( intrinsics ) av[avcnt++] = "-M";
    if ( norecf )   av[avcnt++] = "-r";
    if ( noinvar )  av[avcnt++] = "-v";
    if ( nocse )    av[avcnt++] = "-c";
    if ( noifuse )  av[avcnt++] = "-u";
    if ( nosfuse )  av[avcnt++] = "-j";
    if ( nosplit )  av[avcnt++] = "-S";
    if ( noinvert)  av[avcnt++] = "-Z";
    if ( nodfuse )  av[avcnt++] = "-z";
    if ( nodope )   av[avcnt++] = "-D";
    if ( noamove )  av[avcnt++] = "-9";
    if ( nogcse )   av[avcnt++] = "-g";
    if ( nofold )   av[avcnt++] = "-f";
    if ( inter )    av[avcnt++] = "-t";

    if ( explode ) {
      if ( explodeI )
	av[avcnt++] = "-+";
      else
	av[avcnt++] = "-T";
    }

    if ( inlineall ) av[avcnt++] = "-$";

    if ( noOinvar ) av[avcnt++] = "-V";

    if ( noscalar ) av[avcnt++] = "-a";

    if ( notgcse )  av[avcnt++] = "-1";

    if ( info ) {
      sprintf(dashi,"-i%d",info);
      av[avcnt++] = dashi;
    }
    if ( vinfo || cinfo ) {
      sprintf(dashI,"-I%d",vinfo|cinfo);
      av[avcnt++] = dashI;
    }

    if ( !stopDI ) {
      av[avcnt++] = "-l";
      av[avcnt++] = "-e";
    }
    else 
      av[avcnt++] = "-s";

    if ( iter != NULL )
      av[avcnt++] = iter;

    av[avcnt++] = unroll;

    av[avcnt] = NULL;

    /* ------------------------------------------------------------ */
    /* Launch the optimizer					    */
    /* ------------------------------------------------------------ */
    status = Submit(if1opt);
    RemoveFile( file );
    if ( status != OK ) {
      RemoveFile(tmp0);
      return ERROR;
    }

    file = tmp0;

    if ( stopDI ) return( OK );

    /*******************************************************************/
  case OPT:
    if ( stopOPT ) return( OK );

    av[0] = if2mem;
    av[1] = file;
    av[2] = tmp1;
    RegisterTempFile(tmp1);

    avcnt = 3;

    for(pat=0; pat<PatchCount; pat++) av[avcnt++] = Patches[pat];

    if ( !Warnings ) av[avcnt++] = "-w";
    if ( Prof ) av[avcnt++] = "-W";

    if ( !noscalar ) {
      if ( noinvar )  av[avcnt++] = "-v";
      if ( noOinvar ) av[avcnt++] = "-V";
      if ( nocse )    av[avcnt++] = "-c";
      if ( nogcse )   av[avcnt++] = "-g";
      if ( nofold )   av[avcnt++] = "-f";
    }
    else
      av[avcnt++] = "-a";

    if ( sdbx ) av[avcnt++] = "-^";

    if ( SISglue ) av[avcnt++] = "-8";

    if ( sdbx ) av[avcnt++] = "-a";

    if ( info ) {
      sprintf(dashi,"-i%d",info);
      av[avcnt++] = dashi;
    }

    if ( noif2mem )
      av[avcnt++] = "-m";

    av[avcnt] = NULL;

    /* ------------------------------------------------------------ */
    /* Launch the memory manager				    */
    /* ------------------------------------------------------------ */
    status = Submit(if2mem);
    RemoveFile( file );
    if ( status != OK ) {
      RemoveFile(tmp1);
      return ERROR;
    }

    file = tmp1;
    /*******************************************************************/
  case MEM:
    if ( stopMEM ) return( OK );

    av[0] = if2up;
    av[1] = file;
    av[2] = tmp2;
    RegisterTempFile(tmp2);

    avcnt = 3;

    for(pat=0; pat<PatchCount; pat++) av[avcnt++] = Patches[pat];

    if ( info ) {
      sprintf(dashi,"-i%d",info);
      av[avcnt++] = dashi;
    }

    if ( !Warnings ) av[avcnt++] = "-w";
    if ( Prof ) av[avcnt++] = "-W";

    if ( noif2up )
      av[avcnt++] = "-m";

    if ( noif2up || nostr )
      av[avcnt++] = "-t";

    if ( nocagg )
      av[avcnt++] = "-c";

    if ( noif2up || nomig )
      av[avcnt++] = "-l";

    if ( sdbx ) av[avcnt++] = "-^";

    if ( !nocom )
      av[avcnt++] = "-I";

    av[avcnt] = NULL;

    /* ------------------------------------------------------------ */
    /* Launch the update in place  manager			    */
    /* ------------------------------------------------------------ */
    status = Submit(if2up);
    RemoveFile( file );
    if ( status != OK ) {
      RemoveFile(tmp2);
      return ERROR;
    }

    file = tmp2;
    /*******************************************************************/
  case UP:
    if ( stopUP ) return( OK );

    av[0] = if2part;
    av[1] = costs;
    av[2] = file;
    av[3] = tmp3;
    av[4] = LoopReportOut;
    av[5] = LoopReportIn;
    RegisterTempFile(tmp3);

    avcnt = 6;

    for(pat=0; pat<PatchCount; pat++) av[avcnt++] = Patches[pat];

    av[avcnt++] = LoopStyle;

    if ( !Warnings ) av[avcnt++] = "-w";
    if ( Prof ) av[avcnt++] = "-W";

    if ( mdb != NULL ) {
      sprintf( module, "-X%s", mdb );
      av[avcnt++] = module;
    }

    if ( info ) {
      sprintf(dashi,"-i%d",info);
      av[avcnt++] = dashi;
    }

    if ( noassoc ) av[avcnt++] = "-R";

    if ( atlevel != NULL )
      av[avcnt++] = atlevel;

    if ( level != NULL )
      av[avcnt++] = level;

    if ( !novec ) av[avcnt++] = "-v";

    av[avcnt++] = procs;

    if ( cvector )
      av[avcnt++] = "-C5";
#ifdef CRAY
    av[avcnt++] = "-C5";
#endif

    if ( huge != NULL )
      av[avcnt++] = huge;

    if ( iter != NULL )
      av[avcnt++] = iter;

    av[avcnt] = NULL;

    /* ------------------------------------------------------------ */
    /* Launch the partitioner					    */
    /* ------------------------------------------------------------ */
    status = Submit(if2part);
    RemoveFile( file );
    if ( status != OK ) {
      RemoveFile(tmp3);
      return ERROR;
    }

    file = tmp3;
    /*******************************************************************/
  case PART:
    if ( stopPART ) return( OK );

    av[0] = if2gen;
    av[1] = file;
    av[2] = tmp4;
    RegisterTempFile(tmp4);

    avcnt = 3;

    if ( !MinSliceThrottle ) av[avcnt++] = "-m";

    for(pat=0; pat<PatchCount; pat++) av[avcnt++] = Patches[pat];

    if ( !Warnings ) av[avcnt++] = "-w";
    if ( Prof ) av[avcnt++] = "-W";

    if ( ShowSource ) av[avcnt++] = "-%";

    for ( idx = 0; idx <= flopoptioncnt; idx++ ) {
      av[avcnt++] = "-@";
      av[avcnt++] = flopoptions[idx];
    }

    if ( timeall ) {
      av[avcnt++] = "-&&";
    } else {
      for ( idx = 0; idx <= timeoptioncnt; idx++ ) {
	av[avcnt++] = "-&";
	av[avcnt++] = timeoptions[idx];
      }
    }

    if ( traceall ) {
      av[avcnt++] = "-TT";
    } else {
      for ( idx = 0; idx <= traceoptioncnt; idx++ ) {
	av[avcnt++] = "-T";
	av[avcnt++] = traceoptions[idx];
      }
    }

    if ( norag ) av[avcnt++] = "-1";

    if ( BindProcessors ) av[avcnt++] = "-4";

    if ( nobrec ) av[avcnt++] = "-#";

    if ( noassoc ) av[avcnt++] = "-y";

    if ( !bipmv ) av[avcnt++] = "-0";

    if ( hybrid ) {
      sprintf( hybridDOT, "%sF.o", root );
      sprintf( hybridROOT, "%sF", root );
      sprintf( hybridFILE, "%s.f", hybridROOT );
      sprintf( hybridOPTION, "-K%s", hybridROOT );

      av[avcnt++] = hybridOPTION;
      dohybrid = TRUE;
    }

    if ( freeall ) av[avcnt++] = "-9";

    if ( intrinsics ) av[avcnt++] = "-U";

    if ( alliantfx ) av[avcnt++] = "-5";
    if ( cRay )   av[avcnt++] = "-6";
    if ( nltss )     av[avcnt++] = "-7";

    /* #ifdef SGI */
    /* av[avcnt++] = "-x"; */
    /* #endif */

    if ( SISglue ) av[avcnt++] = "-8";

    if ( !nogshared ) av[avcnt++] = "-G";
    if ( movereads ) av[avcnt++] = "-2";

    if ( newchains ) av[avcnt++] = "-3N";
    else if ( chains ) av[avcnt++] = "-3";

    if ( useORTS )
      av[avcnt++] = "-O";

    if ( cvector )
      av[avcnt++] = "-C5";
#ifdef CRAY
    av[avcnt++] = "-C5";
#endif

    if ( avector ) av[avcnt++] = "-A";
#ifdef ALLIANT
    av[avcnt++] = "-A";
#endif

    if ( !novec )  av[avcnt++] = "-X";

    if ( prog )    av[avcnt++] = "-P";

    if ( info ) {
      sprintf(dashi,"-i%d",info);
      av[avcnt++] = dashi;
    }
    if ( cinfo || vinfo ) {
      sprintf(dashV,"-V%d",cinfo|vinfo);
      av[avcnt++] = dashV;
    }

    if ( noregs )  av[avcnt++] = "-r";
    if ( nofcopy ) av[avcnt++] = "-F";
    if ( flt )     av[avcnt++] = "-f";
    if ( dbl )     av[avcnt++] = "-d";
    if ( noaimp )  av[avcnt++] = "-a";

    /* if ( forC )    av[avcnt++] = "-L"; */
    /* if ( forF )    av[avcnt++] = "-l"; */

    if ( nobip )
      av[avcnt++] = "-P";

    av[avcnt++] = preb;

    if ( noif2up )
      av[avcnt++] = "-u";

    if ( sdbx ) {
      av[avcnt++] = "-a";	/* -sdbx implies -noaimp */
      av[avcnt++] = "-{";
    }

    if ( bounds ) {
      av[avcnt++] = "-B";
      av[avcnt++] = "-a";	/* -bounds implies -noaimp */
    }

    av[avcnt] = NULL;

    /* ------------------------------------------------------------ */
    /* Launch the generator					    */
    /* ------------------------------------------------------------ */
    status = Submit(if2gen);
    RemoveFile( file );
    if ( status != OK ) {
      RemoveFile(tmp4);
      return ERROR;
    }

    file = tmp4;

    c[++ccnt] = cfile = file;

    /*******************************************************************/
  case C:
    if ( stopC ) return( OK );
    /*******************************************************************/
  case LD:
    /*******************************************************************/
    if ( dohybrid ) {
      avcnt = 0;
      AddAV(ff);
      AddAV("-c");

#ifdef CRAY
      av[avcnt++] = "-Wf-astack";
#endif
#ifdef ALLIANT
      av[avcnt++] = "-recursive";
#endif

      for ( cnt = 0; cnt <= ffoptionscnt; cnt++ )
	av[avcnt++] = ffoptions[cnt];

      av[avcnt++] = hybridFILE;

      av[avcnt] = NULL;

      if ( Submit( av[0] ) == ERROR )
	return( ERROR );

      ld[++ldcnt] = hybridDOT;
    }
    /*******************************************************************/
    if ( forF && ccnt < 0 ) goto ProcessFORTRAN;

    avcnt = 0;
    AddAV(cc);

#if GNU
    AddAV("-DGNU");
#endif

#ifdef CRAY
    if ( info || vinfo ) {
#if SCC >= 3
      av[avcnt++] = "-h";
      av[avcnt++] = "report=v";
#else
      av[avcnt++] = "-hvreport";
#endif
    }

    if ( noimp ) {
#if SCC >= 3
      av[avcnt++] = "-O0";
#else
      av[avcnt++] = "-hnoopt";
#endif
    } else {
#if SCC >= 3
      av[avcnt++] = "-h";
      av[avcnt++] = "inline0";
      av[avcnt++] = "-h";
      av[avcnt++] = "scalar1";
      av[avcnt++] = "-h";
      av[avcnt++] = "task0";
      av[avcnt++] = "-h";
      av[avcnt++] = "vector0";
#endif
    }

    if ( novec ) {
#if SCC >= 3
      av[avcnt++] = "-h";
      av[avcnt++] = "vector0";
      av[avcnt++] = "-h";
      av[avcnt++] = "nopragma=allvector";
#else
      av[avcnt++] = "-hnovector";
#endif
    }

    if ( !novec ) {
#if SCC >= 3
      av[avcnt++] = "-h";
      av[avcnt++] = "aggress";
#else
      av[avcnt++] = "-haggress";
#endif
    }
#endif

    av[avcnt++] = incl;

    if ( flpinfo )
      av[avcnt++] = "-DFInfo=1";

    if ( cpyinfo )
      av[avcnt++] = "-DCInfo=1";

#ifdef SUN
    av[avcnt++] = "-DSUN";
#endif

#ifdef SGI
    av[avcnt++] = "-DSGI";
#endif

#ifdef RS6000
    av[avcnt++] = "-DRS6000";
#endif

#ifdef SUNIX
    av[avcnt++] = "-DSUNIX";
#endif

#ifdef GANGD
    av[avcnt++] = "-DGANGD";
#endif

#ifdef SYMMETRY
    av[avcnt++] = "-DSYMMETRY";
#endif

#ifdef BALANCE
    av[avcnt++] = "-DBALANCE";
#endif

#ifdef ENCORE
    av[avcnt++] = "-DENCORE";
#endif

#ifdef CRAY
    av[avcnt++] = "-DCRAY";
#ifdef CRAYXY
    av[avcnt++] = "-DCRAYXY";
#endif
#ifdef CRAY2
    av[avcnt++] = "-DCRAY2";
#endif
#endif

#ifdef ALLIANT
    av[avcnt++] = "-DALLIANT";
    av[avcnt++] = "-ce";

    if ( info || vinfo ) {
      av[avcnt++] = "-l";
      av[avcnt++] = "-OMip";
    }
    else
      av[avcnt++] = "-OMipsl";
#endif
    if ( !noimp ) {
#ifndef CRAY
#  ifdef  ALLIANT
      if ( !noassoc )
	av[avcnt++] = "-AS";

      if ( !novec )
	av[avcnt++] = "-Oigv";
      else
	av[avcnt++] = "-Oig";

#  else
#    ifdef SGI
#    if !GNU
      av[avcnt++] = "-O3";
      av[avcnt++] = "-Olimit";
      av[avcnt++] = "999999";
#    else
      av[avcnt++] = "-O";
#    endif
#    else
#      ifdef HPUX
      av[avcnt++] = "+O1";
#      else
      av[avcnt++] = "-O";
#      endif
#    endif
#  endif
#endif

#if BALANCE || SYMMETRY
      av[avcnt++] = "-i";
#endif
    }

#ifdef w1167
#if !GNU
    av[avcnt++] = "-f1167";
#endif
#endif

#ifdef F68881
#if !GNU
    av[avcnt++] = "-f68881";
#endif
#endif

    if ( stopS ) av[avcnt++] = "-S";

    if ( forF || stopc ) av[avcnt++] = "-c";

#if SYMMETRY || BALANCE
    av[avcnt++] = "-w";
#endif

#ifdef ENCORE
    av[avcnt++] = "-q";
    av[avcnt++] = "long_jump";
#endif

#ifdef SGI
#if !GNU
    av[avcnt++] = "-float";
#endif
#endif

    /* ------------------------------------------------------------ */
    /* Add in any special C options specified on command line	    */
    /* ------------------------------------------------------------ */
    for ( cnt = 0; cnt <= ccoptionscnt; cnt++ )
      av[avcnt++] = ccoptions[cnt];

    /* ------------------------------------------------------------ */
    /* Set the output name (unless for fortran!)		    */
    /* ------------------------------------------------------------ */
    if ( !forF && (forC || !(stopc || stopS)) ) {
      av[avcnt++] = "-o";

      if ( aabs == NULL ) {
	if ( forC )
	  av[avcnt++] = "c.out";
	else 
	  av[avcnt++] = "s.out";
      }
      else
	av[avcnt++] = aabs;
    }

    /* ------------------------------------------------------------ */
    /* Add in any C source files (supplied or generated)	    */
    /* ------------------------------------------------------------ */
    for ( idx = 0; idx <= ccnt; idx++ ) av[avcnt++] = c[idx];

    /* ------------------------------------------------------------ */
    /* Add runtime libraries					    */
    /* ------------------------------------------------------------ */
    if ( !forF && (forC || !(stopc || stopS)) ) {
      if ( !forF && !forC )
	av[avcnt++] = srt0;

#ifndef ALLIANT
#ifndef SYMMETRY
#ifndef BALANCE
      av[avcnt++] = libs;
      /* HPUX cc needs a blank between '-L' and path */
#	if HPUX || HPUXPA
#	if !GNU
      av[avcnt++] = BIN_PATH;
#	endif
#	endif
#endif
#endif
#endif

      /* ------------------------------------------------------------ */
      /* User supplied loader options				      */
      /* ------------------------------------------------------------ */
      for ( idx = 0; idx <= ldcnt; idx++ ) av[avcnt++] = ld[idx];

#if ALLIANT || SYMMETRY || BALANCE
      av[avcnt++] = libs;
#else
      av[avcnt++] = "-lsisal";
#endif

#ifdef ALLIANT
      av[avcnt++] = "-lcvec";
      av[avcnt++] = "-lcommon";
#endif

#ifdef CRAY
      av[avcnt++] = "-lu";
      av[avcnt++] = "-lsci";
#endif

#if SGI
      av[avcnt++] = "-lmpc";
#endif

#if BALANCE || SYMMETRY
#ifdef GANGD
      av[avcnt++] = "-lppp";
#endif
      av[avcnt++] = "-lpps";
#endif 

#ifdef ENCORE
      /* av[avcnt++] = "-lu"; */
      av[avcnt++] = "-lpp";
#endif

      av[avcnt++] = "-lm";

#ifndef CRAY
#ifndef SGI
#ifndef NeXT
      av[avcnt++] = "-lc";
#endif
#endif
#endif
    }

    av[avcnt] = NULL;

    /* ------------------------------------------------------------ */
    /* Submit to the C compiler/loader				    */
    /* ------------------------------------------------------------ */
    status = Submit(av[0]);

    /* ------------------------------------------------------------ */
    /* Remove the temporary .c/.o files				    */
    /* ------------------------------------------------------------ */
    for ( idx = 0; idx <= ccnt; idx++ ) {
      char	ofile[MAX_PATH],*dotc;
      if ( strcmp( cfile, c[idx] ) == SAME ) RemoveFile( c[idx] );
      if ( !mdb ) {
	strcpy(ofile,c[idx]);
	dotc = GetFileSuffix(ofile);
	if ( dotc ) {
	  strcpy(dotc,".o");
	  RemoveFile(ofile);
	}
      }
    }
    if ( status != OK ) return ERROR;

    /*******************************************************************/
  ProcessFORTRAN:
    if ( !forF || stopc ) break;

  case F:

    avcnt = 0;
    AddAV(ff);
    AddAV("-o");

    if ( aabs == NULL ) {
      AddAV("f.out");
    } else {
      AddAV(aabs);
    }


#ifdef F68881
    av[avcnt++] = "-f68881";
#endif

#ifdef CRAY
    av[avcnt++] = "-Wf-astack";
#endif

    for ( cnt = 0; cnt <= ffoptionscnt; cnt++ )
      av[avcnt++] = ffoptions[cnt];

    for ( idx = 0; idx <= fcnt; idx++ )
      av[avcnt++] = f[idx];

    for ( idx = 0; idx <= ccnt; idx++ )
      av[avcnt++] = SetFileSuffix("",GetFileRoot( c[idx] ), ".o" );


#ifndef ALLIANT
#ifndef SYMMETRY
#ifndef BALANCE
#ifndef SUNIX
#ifndef RS6000
    av[avcnt++] = libs;
#endif
#endif
#endif
#endif
#endif

    for ( idx = 0; idx <= ldcnt; idx++ )
      av[avcnt++] = ld[idx];

#if ALLIANT || SYMMETRY || BALANCE || SUNIX || RS6000
    av[avcnt++] = libs;
#if SUNIX || RS6000
    av[avcnt++] = "-lsisal";
#endif
#else
    av[avcnt++] = "-lsisal";
#endif

#ifdef ALLIANT
    av[avcnt++] = "-lcvec";
    av[avcnt++] = "-lcommon";
#endif

#ifdef CRAY
    av[avcnt++] = "-lu";
    av[avcnt++] = "-lsci";
#endif

#if SGI
    av[avcnt++] = "-lmpc";
#endif

#if BALANCE || SYMMETRY
#ifdef GANGD
    av[avcnt++] = "-lppp";
#endif
    av[avcnt++] = "-lpps";
#endif 

#ifdef ENCORE
    /* av[avcnt++] = "-lu"; */
    av[avcnt++] = "-lpp";
#endif

    av[avcnt++] = "-lm";

#ifndef CRAY
#ifndef ENCORE
#ifndef SGI
    av[avcnt++] = "-lc";
#endif
#endif
#endif

    av[avcnt] = NULL;

    if ( Submit( av[0] ) == ERROR )
      return( ERROR );

    break;

    /*******************************************************************/
  default:
    break;
  }

  return( OK );
}


/**************************************************************************/
/* GLOBAL **************            main           ************************/
/**************************************************************************/
/* PURPOSE: PARSE THE OSC COMMAND LINE, BUILD EXECUTABLE DIRECTORY PATHS, */
/*          AND CALL SubmitCommands TO REALIZE COMPILATION.               */
/**************************************************************************/

static char VERSION[] = "v12.9.2";
#ifndef VPREFIX
# define VPREFIX ""
#endif

main( argc, argv )
int    argc;
char **argv;
{
  int		i,fake_argc;
  char		*p,*OSC_OPTIONS,*OSC_ENVIRONMENT,*fake_argv[100];
  char		*getenv();
  char		*TMP;

  /* ------------------------------------------------------------ */
  /* Set up signal handlers to clean up dead temporary files	  */
  /* ------------------------------------------------------------ */
#ifdef SIGHUP
  signal(SIGHUP	,DeathWish);
#endif
#ifdef SIGINT
  signal(SIGINT	,DeathWish);
#endif
#ifdef SIGQUIT
  signal(SIGQUIT,DeathWish);
#endif
#ifdef SIGILL
  signal(SIGILL	,DeathWish);
#endif
#ifdef SIGBUS
  signal(SIGBUS	,DeathWish);
#endif
#ifdef SIGSEGV
  signal(SIGSEGV,DeathWish);
#endif
#ifdef SIGXCPU
  signal(SIGXCPU,DeathWish);
#endif

  sprintf( procs = (char *) malloc(100), "-P%d", MAX_PROCS ); 

  /* ------------------------------------------------------------ */
  /* See if there is a temp directory set			  */
  /* ------------------------------------------------------------ */
#ifdef GETTMPDIR
  TMP = getenv("TMPDIR");
  if ( TMP ) TMPDIR = TMP;
#endif

  /* ------------------------------------------------------------ */
  /* First parse any options from the OSC_OPTIONS environment	  */
  /* ------------------------------------------------------------ */
  OSC_OPTIONS = getenv("OSC_OPTIONS");
  if ( OSC_OPTIONS ) {
    OSC_ENVIRONMENT = malloc(strlen(OSC_OPTIONS)+1);
    strcpy(OSC_ENVIRONMENT,OSC_OPTIONS);

    /* Convert spaces to nulls and collect count and pointers */
    fake_argv[0] = argv[0];
    fake_argc = 1;
    p = OSC_ENVIRONMENT;
    do {
      /* Convert blanks to null */
      while(*p && isspace(*p)) *(p++) = NULL;
      if ( *p ) {
	/* Set the pointer to the argument */
	fake_argv[fake_argc++] = p;
      while(*p && !isspace(*p)) p++;
      }
    } while (*p);
    fake_argv[fake_argc] = NULL;

    ParseCommandLine( fake_argc, fake_argv );
  }

  /* ------------------------------------------------------------ */
  /* Now parse any command line options				  */
  /* ------------------------------------------------------------ */
  ParseCommandLine( argc, argv );

  /* ------------------------------------------------------------ */
  StrCat( sisal,     BIN_PATH,  "/sisal"     );
  StrCat( sisalpt,   BIN_PATH,  ""           );
  StrCat( if1ld,     BIN_PATH,  "/if1ld"     );
  StrCat( if1opt,    BIN_PATH,  "/if1opt"    );
  StrCat( if2mem,    BIN_PATH,  "/if2mem"    );
  StrCat( if2up,     BIN_PATH,  "/if2up"     );
  StrCat( if2part,   BIN_PATH,  "/if2part"   );
  StrCat( if2gen,    BIN_PATH,  "/if2gen"    );
  StrCat( spprun,    BIN_PATH,  "/spprun"    );
  StrCat( spp2,      BIN_PATH,  "/spp2"      );

    if ( !newff ) {
#ifdef USEFF
      ff = USEFF;
#else
#  if ALLIANT || BALANCE || SYMMETRY
      ff = "fortran";
#  else
#    ifdef CRAY
      ff = "cf77";
#    else
      ff = "f77";
#    endif
#  endif
#endif
      }

    if ( !newcc ) {
#ifdef USECC
      cc = USECC;
#else
#  ifdef ALLIANT
      cc = "fxc";
#  else
#    ifdef CRAY
      cc = "cc";
#    else
      cc = "cc";
#    endif
#  endif
#endif
      }

    StrCat( incl,      "-I",          BIN_PATH );

#if ALLIANT || SYMMETRY || BALANCE
    StrCat( libs,      BIN_PATH,      "/libsisal.a" );
#else

#  if HPUX || HPUXPA
   /* HPUX cc needs blank between '-L' and path */
#    if GNU
       StrCat( libs,      "-L",          BIN_PATH );
#    else
       strcpy( libs,      "-L");
#    endif
#  else
     StrCat( libs,      "-L",          BIN_PATH );
#  endif

#endif

    StrCat( srt0,      BIN_PATH,  "/p-srt0.o"    );

    if ( costs == NULL ) {
        costs = buf1;
        StrCat( costs,   BIN_PATH,  "/s.costs"   );
        }

    if ( SubmitCommands() == ERROR ) {
        fprintf( stderr, "\n ** COMPILATION ABORTED ** \n\n" );
        exit( ERROR );
        }
        
    exit( OK );
}

/* $Log: osc.c,v $
 * Revision 1.16  1993/11/30  00:51:46  miller
 * Death wish to clean up trash files
 *
 * Revision 1.15  1993/06/10  18:38:07  miller
 * Allow override of looking for TMPDIR env. var.
 *
 * Revision 1.14  1993/06/02  23:51:53  miller
 * Fixing the name reported in the .if1 file (caused by TMPDIR stuff).
 *
 * Revision 1.13  1993/04/16  17:16:09  miller
 * Temporary directory
 * Warning suppression
 * Bounds checking on by default
 *
 * Revision 1.12  1993/04/01  23:36:48  miller
 * added a line of stars
 *
 * Revision 1.11  1993/03/23  22:58:00  miller
 * Fixed problem with rejection of absolute names without extension
 *
 * Revision 1.10  1994/03/18  18:26:09  miller
 * Had the logic for the -O option wrong.  Added support for an OSC_OPTIONS
 * environment variable.  It is parsed with ParseCommandLine just like the
 * argc,argv stuff is.  There is a silent limit of 100 options that may
 * be specified.  This should be changed to be option_count.
 *
 * Revision 1.9  1994/03/11  18:14:52  miller
 * Changed mind on the code improvers.  Now the only thing turned
 * off is the object code improver.
 *
 * Revision 1.8  1994/03/10  00:56:23  miller
 * Added comments regarding what the global variables are.  Also modified
 * to make non-optimized code the default (see new -O option).
 * */
