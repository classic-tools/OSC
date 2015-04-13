/* if2count.c,v
 * Revision 12.7  1992/11/04  22:05:11  miller
 * Initial revision
 *
 * Revision 12.7  1992/10/21  18:10:01  miller
 * Initial RCS Version by Cann
 * */

#include "world.h"


static int noops;  /* COUNT OF NoOp NODES                                 */
static int snoops; /* COUNT OF DEACTIVE (SYNC-ONLY) NoOp NODES            */
static int rnoops; /* COUNT OF CONDITIONAL NoOp NODES                     */
static int pms;    /* COUNT OF ACTIVE pm PRAGMAS                          */
static int pls;    /* COUNT OF ACTIVE pl PRAGMAS                          */
static int srs;    /* COUNT OF ACTIVE sr PRAGMAS                          */
static int cms;    /* COUNT OF ACTIVE cm PRAGMAS                          */
static int bades;  /* COUNT OF BOUND ADES                                 */
static int lades;  /* COUNT OF LOW PRIORITY ADES                          */
static int hades;  /* COUNT OF HIGH PRIORITY ADES                         */
static int cagnodes; /* COUNT OF CONSTANT AGGREGATE GENERATORS            */
static int ds;     /* COUNT OF mk=D PRAGMAS                               */
static int dedges; /* COUNT OF SCOPE EXIT RECYCLE EDGES                   */
       int hnoops; /* COUNT OF HOISTED NoOp NODES                         */

static int last;   /* HAS OPTIMIZATION COMPLETED?                         */


/**************************************************************************/
/* LOCAL  **************       GatherCounts        ************************/
/**************************************************************************/
/* PURPOSE: GATHER THE OCCURENCE COUNTS FOR GRAPH g.  COUNTED ITEMS       */
/*          INCLUDE ACTIVE REFERENCE COUNT PRAGMAS, NoOp NODES, CONSTANT  */
/*          AGGREGATE CONSTRUCTORS, AND ARTIFICIAL DEPENDENCE EDGES.      */
/**************************************************************************/

static void GatherCounts( g )
PNODE g;
{
    register PNODE n;
    register PEDGE e;
    register PEDGE i;
    register PADE  a;
    register PEDGE ee;

    for ( n = g; n != NULL; n = n->nsucc ) {
	for ( i = n->imp; i != NULL; i = i->isucc ) {
	    if ( i->cm != 0 )
		cms++;

	    if ( i->dmark )
		ds++;

	    if ( i->iport == 0 )
		dedges++;
            }

        for ( e = n->exp; e != NULL; e = e->esucc ) {
	    if ( IsAggregate( e->info ) && (e->eport >= 0) ) {
		if ( e->sr > 0 )
		    srs++;
                else if ( e->pm > 0 )
		    pms++;
      
		if ( e->pl > 0 )
		    pls++;

                /* INVALIDATE OTHER REFERENCES SO ONLY COUNTED ONCE       */

	        for ( ee = e->esucc; ee != NULL; ee = ee->esucc )
		    if ( ee->eport == e->eport )
		        ee->eport = -(ee->eport);
		}

	    e->eport = abs( e->eport );
	    }

	for ( a = n->aimp; a != NULL; a = a->isucc )
	    switch ( a->priority ) {
	       case BOUND:
		   bades++;
		   break;

               case HIGH_PRI:
		   hades++;
		   break;

               default:
		   lades++;
		   break;
               }

	switch ( n->type ) {
	    case IFDefArrayBuf:
		if ( !(n->cmark) )
		    break;

                if ( !last )
		    break;
		
                FPRINTF( stderr, " Constant Aggregate Generator (%s):\n", 
				 cfunct->G_NAME                        );

                FPRINTF( stderr, "     %s (%s,%s,%d)\n", GetNodeName( n ), 
				 n->file, n->funct, n->line             );

                FPRINTF( stderr, "     %s (%s,%s,%d)\n", 
				 GetNodeName( n->exp->dst ), n->exp->dst->file,
				 n->exp->dst->funct, n->exp->dst->line       );

                FPRINTF( stderr, "     %s (%s,%s,%d)\n\n", 
				 GetNodeName( n->exp->dst->exp->dst ),
				 n->exp->dst->exp->dst->file,
				 n->exp->dst->exp->dst->funct,
				 n->exp->dst->exp->dst->line        );

		cagnodes +=3;
		break;

	    case IFRBuild:
		if ( !(n->cmark) )
		    break;

		if ( !last )
		    break;


                FPRINTF( stderr, " Constant Aggregate Generator %s (%s): ", 
				 GetNodeName( n ), cfunct->G_NAME         );

                FPRINTF( stderr, " (%s,%s,%d)\n\n", n->file, n->funct, n->line);

		cagnodes++;
                break;

	    case IFNoOp:
		noops++;

                FPRINTF( stderr, " NoOp Node (%s): ", cfunct->G_NAME );

		if ( IsLoop( n->exp->dst ) )
		  FPRINTF( stderr, " (HOISTED FROM LOOP) " );

		switch ( n->exp->info->type ) {
		    case IF_ARRAY:
			FPRINTF( stderr, "(ARRAY) " );
			break;
		    case IF_STREAM:
			FPRINTF( stderr, "(STREAM) " );
			break;
		    case IF_RECORD:
			FPRINTF( stderr, "(RECORD) " );
			break;
		    default: break;
		    }

		if ( n->imp->pmark ) {
		    switch ( n->imp->rmark1 ) {
			case RMARK:
			    if ( n->imp->omark1 )
                                FPRINTF( stderr, "mk=PRO, SYNC-ONLY" );
                            else
                                FPRINTF( stderr, "mk=PR,  SYNC-ONLY" );

			    snoops++;
			    break;

			case rMARK:
			    if ( n->imp->omark1 )
                                FPRINTF( stderr, "mk=PrO, POSSIBLE DOPE COPY ");
			    else
                                FPRINTF( stderr, "mk=Pr, POSSIBLE DOPE COPY " );

			    rnoops++;
			    break;

			default:
			    if ( n->imp->omark1 )
			        FPRINTF( stderr, "mk=PO, DOPE COPY" );
                            else
			        FPRINTF( stderr, "mk=P, DOPE COPY"  );

			    break;
                        }

                    FPRINTF( stderr, " (%s,%s,%d)\n\n", n->file, 
							n->funct, n->line);
		    break;
		    }

		switch ( n->imp->rmark1 ) {
		    case RMARK:
			if ( n->imp->omark1 ) {
                            FPRINTF( stderr, "mk=RO, SYNC-ONLY" );
			    snoops++;
			    }
                        else
                            FPRINTF( stderr, "mk=R, POSSIBLE PHYS COPY" );

			break;

		    case rMARK:
			if ( n->imp->omark1 )
                            FPRINTF( stderr, "mk=rO, POSSIBLE DOPE-PHYS COPY" );
                        else
                            FPRINTF( stderr, "mk=r, POSSIBLE DOPE-PHYS COPY" );

			rnoops++;
			break;

		    default:
			if ( n->imp->omark1 )
                            FPRINTF( stderr, "mk=O, DOPE-PHYS COPY" );
                        else
                            FPRINTF( stderr, "mk=, DOPE-PHYS COPY " );

			break;
                    }

                FPRINTF( stderr, " (%s,%s,%d)\n\n", n->file, n->funct, n->line);
		break;

	    case IFForall:
	    case IFLoopA:
	    case IFLoopB:
	    case IFSelect:
	    case IFTagCase:
		for ( g = n->C_SUBS; g != NULL; g = g->gsucc )
		    GatherCounts( g );

		break;

	    default:
		break;
            }
	}
}


/**************************************************************************/
/* LOCAL  **************      WriteUpCountInfo       ************************/
/**************************************************************************/
/* PURPOSE: WRITE OCCURENCE COUNTS TO output.                             */
/**************************************************************************/

static void WriteUpCountInfo()
{
    FPRINTF( stderr,   " NoOp Nodes:                    %d\n", noops   );
    FPRINTF( stderr,   " Sync-Only NoOp Nodes:          %d\n", snoops  );
    FPRINTF( stderr,   " Conditional NoOp Nodes:        %d\n", rnoops  );
    FPRINTF( stderr,   " Hoisted NoOp Nodes:            %d\n", hnoops  );
    FPRINTF( stderr,   " Active sr Pragmas:             %d\n", srs     );
    FPRINTF( stderr,   " Active pm Pragmas:             %d\n", pms     );
    FPRINTF( stderr,   " Active pl Pragmas:             %d\n", pls     );
    FPRINTF( stderr,   " Active cm Pragmas:             %d\n", cms     );
    FPRINTF( stderr,   " Bound Ades:                    %d\n", bades   );
    FPRINTF( stderr,   " Low Priority Ades:             %d\n", lades   );
    FPRINTF( stderr,   " High Priority Ades:            %d\n", hades   );
    FPRINTF( stderr,   " ADEs Not Inserted (Cycles):    %d\n", cycle   );
    FPRINTF( stderr,   " Constant Aggregate Generators: %d\n", cagnodes);
    FPRINTF( stderr,   " Active mk=D Pragmas:           %d\n", ds      );
    FPRINTF( stderr,   " Scope Exit Recycle References: %d\n", dedges  );
    FPRINTF( stderr,   " Migrated Nodes:                %d\n", cmig    );
    FPRINTF( stderr,   " Universal Record Ownership:    TRUE\n"        );

    FPRINTF( stderr,   " Universal Stream Ownership:    %s\n", 
		       ( univso )? "TRUE" : "DON'T KNOW"    );

    FPRINTF( stderr,   " Universal Array Ownership:     %s\n", 
		       ( univao )? "TRUE" : "DON'T KNOW"    );

    FPRINTF( stderr,   " Swap Optimizations:            %d\n", swcnt   );
}


/**************************************************************************/
/* GLOBAL **************        UpIf2Count         ************************/
/**************************************************************************/
/* PURPOSE: GATHER OCCURENCE COUNTS FOR ALL FUNCTION GRAPHS AND WRITE THE */
/*          RESULTS, ALONG WITH QUALIFYING MESSAGE msg, TO output.        */
/**************************************************************************/

void UpIf2Count( lst, msg )
int   lst;
char *msg;
{
    register PNODE f;

    FPRINTF( stderr, "\n   * OCCURRENCE COUNTS %s\n\n", msg );

    bades = hades  = lades = srs = cagnodes = 0;
    noops = snoops = pms   = pls = cms    = 0;
    last  = lst;

    for ( f = fhead; f != NULL; f = f->gsucc )
	GatherCounts( cfunct = f );

    WriteUpCountInfo();
}


/**************************************************************************/
/* LOCAL  **************     UpWriteGraphWarnings    ************************/
/**************************************************************************/
/* PURPOSE: WRITE GRAPH WARNINGS FOR g TO stderr.                         */
/**************************************************************************/

static void UpWriteGraphWarnings( g )
PNODE g;
{
    register PNODE  n;
    register PNODE  nn;
    register PNODE  sg;
    register char  *op;

    for ( n = g; n != NULL; n = n->nsucc ) {
      if ( IsCompound( n ) ) {
	for ( sg = n->C_SUBS; sg != NULL; sg = sg->gsucc )
	  UpWriteGraphWarnings( sg );

	continue;
	}

      switch ( n->type ) {
        case IFAAdjust:
	  op = "array_adjust";
	  break;

        case IFARemH:
	  op = "array_remh";
	  break;

        case IFARemL:
	  op = "array_reml";
	  break;

        case IFAReplace:
	  op = "replace";
	  break;

        case IFASetL:
	  op = "array_setl";
	  break;

	default:
	  continue;
        }

      nn = n->imp->src;
      if ( nn->type != IFNoOp )  continue;

      switch ( nn->imp->rmark1 ) {
        case RMARK:
          break;

        case rMARK:
          PrintWarning( op, n->line, n->funct, n->file, FALSE );
          break;

        default:
          PrintWarning( op, n->line, n->funct, n->file, TRUE );
          break;
        }
      }
}


/**************************************************************************/
/* GLOBAL **************    WriteIf2upWarnings     ************************/
/**************************************************************************/
/* PURPOSE: WRITE COPY WARNINGS TO stderr FOR ALL THE FUNCTIONS.          */
/**************************************************************************/

void WriteIf2upWarnings()
{
  register PNODE f;

  for ( f = fhead; f != NULL; f = f->gsucc )
    UpWriteGraphWarnings( f );
}
