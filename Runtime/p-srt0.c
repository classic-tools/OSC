#include "world.h"

static char VERSION[] = "v12.9.2";

main( argc, argv )
     int argc;
     char **argv;
{
  int		i;

  ParseCommandLine( argc, argv );
  InitSisalRunTime();

  FPRINTF( stderr, "%s %s\n", BANNER, VERSION );

  SisalMainArgs = ReadFibreInputs();

  StartWorkers();
  SisalMain( SisalMainArgs );
  StopWorkers();

  if ( !NoFibreOutput ) {
    WriteFibreOutputs( SisalMainArgs );
    fputc( '\n', FibreOutFd );
  }

  if ( GatherPerfInfo ) DumpRunTimeInfo();

  ShutDownDsa();

  exit(0);
}
/* $Log: p-srt0.c,v $
 * Revision 1.8  1993/11/30  00:49:33  miller
 * Removed automagic RCS version number.
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

