#include "world.h"

/* $Log: p-srt0.c,v $
 * Revision 1.12  1994/04/07  23:43:23  denton
 * Put text after cpp # directives in a comment for ANSI.
 *
 * Revision 1.11  1994/04/01  17:02:06  denton
 * NULL -> '\0' where appropriate
 *
 * Revision 1.10  1994/03/24  22:39:46  denton
 * Distributed DSA, non-coherent cache, non-static shared memory
 *
 * Revision 1.7  1993/06/08  17:08:59  miller
 * Trailing newline to force formatted output to flush last line with a
 * newline.
 *
 * Revision 1.6  1993/06/02  23:49:49  miller
 * Broke the lines to help with debug stepping.
 *
 * Revision 1.5  1993/04/16  17:12:11  miller
 * no change
 *
 * Revision 1.4  1993/02/23  23:45:54  miller
 * lint fixes.
 *
 * Revision 1.3  1992/11/11  22:31:14  miller
 * Problem with using RCS version for banner.  It always uses the RCS version
 * number rather than the CVS version.  I have changed it to use the State
 * flag instead.
 *
 * Revision 1.2  1992/11/06  19:24:20  miller
 * Auto version update for banner
 *
 * Revision 12.8  1992/11/05  23:13:11  miller
 * Auto version update for banner
 * */

static char RCSVERSION[] = "$State: V13_0 $";
static char VERSION[SIZEOF(RCSVERSION)] = "?.?";

#if defined(POWER4)


/*
 * $Header: /usr/new/sisal/Master/OSC/Runtime/p-srt0.c,v 1.12 1994/04/07 23:43:23 denton V13_0 $
 *
 */

extern struct shared_s LSR;

char bbbuf[64*1024];

main(int argc, char *argv[])
{
	extern	p6k_fmain();
	extern	p6k_fmaster();
	extern	p6k_envdef();
	int	i;
	char *outstr;
	char me[255];
	char *dbx;

	SRp = &LSR;

  	if(ParseCommandLine( argc, argv ))
	{
		outstr = malloc(255);
		sprintf(outstr,"GEN_NCPUS=%d",NumWorkers);
		putenv(outstr);
		outstr = malloc(255);
		sprintf(outstr,"GEN_CPU1=junior2");
		putenv(outstr);
		outstr = malloc(255);
		sprintf(outstr,"GEN_CPU2=junior3");
		putenv(outstr);
		outstr = malloc(255);
		sprintf(outstr,"GEN_CPU3=junior4");
		putenv(outstr);

	}

	/*
	 * if we are running under DBX, set flag so we can ignore
	 * SIGINT
	 */
        dbx = getenv("GEN_DBX");
	if(dbx != NULL)
		UnderDBX = 1;
	else
		UnderDBX = 0;

	i = p6k_main(argc,argv,p6k_fmain,p6k_fmaster);
	return(i);
}


p6k_fmaster( argc, argv )
int argc;
char **argv;
{
  int		i;

  InitSisalRunTime();

 /*
  * here, we are have called p6k_main.  Copy into shared
  * region
  */
  bcopy(&LSR,&SR,sizeof(LSR));


  /* Form the version number from the RCS checkout revision number */
  if ( RCSVERSION[6] == ':' ) {
    strcpy(VERSION,RCSVERSION+8);
    for(i=0; VERSION[i]; i++) {
      if ( VERSION[i] == '$' ) {
	VERSION[i] = '\0';
	break;
      }
    }
  }

}

p6k_fmain( argc, argv )
int argc;
char **argv;
{

  SisalMainArgs = ReadFibreInputs();

  StartWorkers();

  if ( !NoFibreOutput ) {
    WriteFibreOutputs( SisalMainArgs );
    fputc( '\n', FibreOutFd );
  }

  if ( GatherPerfInfo )
    DumpRunTimeInfo();

  ShutDownDsa();

  exit(0);
}
#else /* POWER4 */

main( argc, argv )
     int argc;
     char **argv;
{
  int		i;

  ParseCommandLine( argc, argv );
  InitSisalRunTime();

  /* Form the version number from the RCS checkout revision number */
  if ( RCSVERSION[6] == ':' ) {
    strcpy(VERSION,RCSVERSION+8);
    for(i=0; VERSION[i]; i++) {
      if ( VERSION[i] == '$' ) {
	VERSION[i] = '\0';
	break;
      }
    }
  }

  FPRINTF( stderr, "%s %s\n", BANNER, VERSION );

  SisalMainArgs = ReadFibreInputs();

  StartWorkers();
  SisalMain( SisalMainArgs );
  StopWorkers();

  if ( !NoFibreOutput ) {
    WriteFibreOutputs( SisalMainArgs );
    fputc( '\n', FibreOutFd );
  }

  if ( GatherPerfInfo )
    DumpRunTimeInfo();

  ShutDownDsa();

  exit(0);
}
#endif
