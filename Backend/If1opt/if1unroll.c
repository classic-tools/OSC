/* if1unroll.c,v
 * Revision 12.7  1992/11/04  22:04:59  miller
 * Initial revision
 *
 * Revision 12.7  1992/10/21  18:08:38  miller
 * Initial RCS Version by Cann
 * */

#include "world.h"

int maxunroll = 2;  /* MAX NUMBER OF ITERATIONS ALLOWED IN AN UNROLLED LOOP */
                    /* CANNOT BE SET TO ZERO                                */

#define MAX_UNROLL 100  /* THE ABSOLUTE LARGEST VALUE ALLOWED FOR maxunroll */

#define MAX_UNROLL_SIZE 150.0                           /* UNROLL THRESHOLD */

int ucnt = 0;                                    /* COUNT OF UNROLLED LOOPS */

double iter = 100.0;                        /* DEFAULT LOOP ITERATION VALUE */

static int lo, hi, num;                /* GLOBAL LOOP INFORMATION VARIABLES */


/**************************************************************************/
/* STATIC **************   ForallIterationCount    ************************/
/**************************************************************************/
/* PURPOSE: RETURN THE ESTIMATED OR REAL ITERATION COUNT OF Forall NODE f.*/
/**************************************************************************/

static double ForallIterationCount( f )
PNODE f;
{
  register PNODE r;
  register int   l;
  register int   h;

  r = f->F_GEN->G_NODES;

  if ( r->type != IFRangeGenerate )
    return( iter );

  if ( !IsConst( r->imp ) )
    return( iter );

  if ( !IsConst( r->imp->isucc ) )
    return( iter );

  l = atoi( r->imp->CoNsT );
  h = atoi( r->imp->isucc->CoNsT );

  return( (double) (h-l+1) );
}


/**************************************************************************/
/* LOCAL  **************         CostCount         ************************/
/**************************************************************************/
/* PURPOSE: DERIVE AN EXECUTION COST ESTIMATE FOR GRAPH g.                */
/**************************************************************************/

static double CostCount( g )
PNODE  g;
{
  register PNODE sg;
  register PNODE n;
  register double cnt;
  register double cnt2;
  register double c;

  cnt = 1.0;

  for ( n = g->G_NODES; n != NULL; n = n->nsucc ) {
    if ( cnt > MAX_UNROLL_SIZE )
      return( cnt );

    switch ( n->type ) {
      case IFLoopA:
      case IFLoopB:
	return( MAX_UNROLL_SIZE );

      case IFForall:
        if ( (cnt2  = ForallIterationCount(n)) >= MAX_UNROLL_SIZE )
	  return( MAX_UNROLL_SIZE );

        cnt  += (CostCount( n->F_BODY ) * cnt2);
	break;

      case IFSelect:
      case IFTagCase:
        c = cnt2 = 0.0;

        for ( sg = n->C_SUBS; sg != NULL; sg = sg->gsucc ) {
          cnt2 += CostCount( sg );
          c++;
          }
          
        cnt += (cnt2/c);
        break;

      default:
	cnt++;
	break;
      }
    }

  return( cnt );
}


/**************************************************************************/
/* LOCAL  **************     AllocMutateNode       ************************/
/**************************************************************************/
/* PURPOSE: RETURN THE APPROPRIATE MUTATE NODE FOR RETURN SUBGRAPH NODE n.*/
/**************************************************************************/

static PNODE AllocMutateNode( n )
PNODE n;
{
  register PNODE m;

  switch ( n->imp->CoNsT[0] ) {
    case REDUCE_GREATEST:
      m = NodeAlloc( ++maxint, IFMax );
      break;

    case REDUCE_LEAST:
      m = NodeAlloc( ++maxint, IFMin );
      break;

    case REDUCE_PRODUCT:
      m = NodeAlloc( ++maxint, IFTimes );
      break;

    case REDUCE_SUM:
      m = NodeAlloc( ++maxint, IFPlus );
      break;

    default:
      Error2( "AllocMutateNode", "ILLEGAL REDUCTION NODE ENCOUNTERED" );
    }

  CopyVitals( n, m );
  return( m );
}


void WriteUnrollInfo()
{
  FPRINTF( stderr, "\n   * LOOP UNROLLING\n\n" );
  FPRINTF( stderr, " Unrolled Foralls: %d\n", ucnt );
}


static int IsUnrollCandidate( f )
PNODE f;
{
  register PNODE  r;
  register PNODE  n;
  register PEDGE  i;
  register double cnt;

  /* IS THE GENERATE A CONSTANT RangeGenerate THAT MEETS THE REQUIREMENTS */
  /* FOR UNROLLING */
  r = f->F_GEN->G_NODES;

  if ( r->nsucc != NULL )
    return( FALSE );

  if ( r->type != IFRangeGenerate )
    return( FALSE );

  if ( !IsConst( r->imp ) )
    return( FALSE );

  if ( !IsConst( r->imp->isucc ) )
    return( FALSE );

  lo = atoi( r->imp->CoNsT );
  hi = atoi( r->imp->isucc->CoNsT );

  num = hi - lo + 1;

  cnt = CostCount( f->F_BODY );

  if ( num <= 0 )
    return( FALSE );

  if ( num > maxunroll ) {
    /* TO PREVENT c ARRAY OVERFLOW!!! */
    if ( num >= MAX_UNROLL )
      return( FALSE );

    if ( ((double)num) * cnt > MAX_UNROLL_SIZE )
      return( FALSE );
    }

  /* ABORT IF CONSTANT PROPAGATION FAILED! */
  for ( i = f->imp; i != NULL; i = i->isucc )
    if ( IsConst( i ) )
      return( FALSE );

  /* DOES THE BODY SUBGRAPH MEET THE REQUIREMENTS FOR UNROLLING */
  for ( i = f->F_BODY->imp; i != NULL; i = i->isucc ) {
    if ( !IsExport( f->F_RET, i->iport ) )
      return( FALSE );

    if ( IsConst( i ) )
      return( FALSE );

    if ( IsSGraph( i->src ) )
      return( FALSE );
    }

  /* DOES THE RETURN SUBGRAPH MEET THE REQUIREMENTS FOR UNROLLING */
  for ( n = f->F_RET->G_NODES; n != NULL; n = n->nsucc )
    switch ( n->type ) {
      case IFAGather:
	/* FILTER? */
	if ( n->imp->isucc->isucc != NULL )
	  return( FALSE );

	/* LOWER BOUND A CONSTANT? */
	if ( !IsConst( n->imp ) )
	  return( FALSE );

	/* VALUE IMPORTED FROM BODY? */
	if ( IsConst( n->imp->isucc ) )
	  return( FALSE );
	if ( !IsImport( f->F_BODY, n->imp->isucc->eport ) )
	  return( FALSE );

	/* NO RETURN SUBGRAPH FANOUT FOR THE VALUE */
	if ( UsageCount( f->F_RET, n->imp->isucc->eport ) != 1 )
	  return( FALSE );

	break;

      case IFReduce:
      case IFRedLeft:
      case IFRedRight:
      case IFRedTree:
	switch ( n->imp->CoNsT[0] ) {
	  case REDUCE_GREATEST:
	  case REDUCE_LEAST:
	  case REDUCE_PRODUCT:
	  case REDUCE_SUM:
	    /* TWO OR MORE ITERATIONS? */
	    if ( num <= 1 )
	      return( FALSE );

	    break;

	  default:
	    return( FALSE );
          }

	/* FILTER? */
	if ( n->imp->isucc->isucc->isucc != NULL )
	  return( FALSE );

	/* VALUE IMPORTED FROM BODY? */
	if ( IsConst( n->imp->isucc->isucc ) )
	  return( FALSE );
	if ( !IsImport( f->F_BODY, n->imp->isucc->isucc->eport ) )
	  return( FALSE );

	/* NO RETURN SUBGRAPH FANOUT FOR THE VALUE */
	if ( UsageCount( f->F_RET, n->imp->isucc->isucc->eport ) != 1 )
	  return( FALSE );

	break;

      default:
	return( FALSE );
      }

  return( TRUE );
}


static void UnrollForalls( g )
PNODE g;
{
  register PNODE sg;
  register PNODE n;
  register PNODE nn;
  register PEDGE e;
  register PEDGE i;
  register PEDGE se;
  register PNODE b;
  register PNODE ag;
  register PNODE sn;
  register PNODE snn;
  register char  *r;
  register PNODE pr;
  register PNODE pb;
	   PNODE c[MAX_UNROLL];
  register int   idx;
           char  s[100];

  for ( n = g->G_NODES; n != NULL; n = snn ) {
    snn = n->nsucc;

    if ( IsCompound( n ) )
      PropagateConst( n );

    /* if ( IsCompound( n ) && !IsForall( n ) ) */
    if ( IsCompound( n ) )
      for ( sg = n->C_SUBS; sg != NULL; sg = sg->gsucc )
	UnrollForalls( sg );

    if ( !IsForall( n ) )
      continue;
    
    if ( !IsUnrollCandidate( n ) )
      continue;

    c[0] = n;
    for ( idx = 1; idx < num; idx++ )
      c[idx] = CopyNode( n );

    /* MUTATE RETURN SUBGRAPHS */
    for ( ag = c[0]->F_RET->G_NODES; ag != NULL; ag = ag->nsucc ) {
      switch ( ag->type ) {
	case IFAGather:
          b = NodeAlloc( ++maxint, IFABuild );
	  CopyVitals( ag, b );

          for ( idx = 0; idx < num; idx++ ) {
	    e = FindImport( c[idx]->F_BODY, ag->imp->isucc->eport );
	    UnlinkImport( e );
	    e->iport = idx + 2;
	    LinkImport( b, e );
	    }

          for ( e = c[0]->exp; e != NULL; e = se ) {
	    se = e->esucc;

	    if ( e->eport == ag->exp->iport ) {
	      UnlinkExport( e );
	      e->eport = 1;
	      LinkExport( b, e );
	      }
            }

	  /* WIRE LOWER BOUND */
          /* IsUnrollCandidate: MUST BE A CONSTANT */
          UnlinkImport( e = ag->imp );
          e->iport = 1;
          LinkImport( b, e );
          LinkNode( n, b );

	  break;

        case IFReduce:
        case IFRedLeft:
        case IFRedRight:
        case IFRedTree:
	  /* WIRE THE FIRST MUTATE NODE */
	  b = AllocMutateNode( ag );
	  LinkNode( n, b );

	  e = FindImport( c[0]->F_BODY, ag->imp->isucc->isucc->eport );
	  UnlinkImport( e );
	  e->iport = 1;
	  LinkImport( b, e );

	  e = FindImport( c[1]->F_BODY, ag->imp->isucc->isucc->eport );
	  UnlinkImport( e );
	  e->iport = 2;
	  LinkImport( b, e );

	  /* WIRE THE REMAINING MUTATE NODES */
	  pb = b;
          for ( idx = 2; idx < num; idx++ ) {
	    b = AllocMutateNode( ag );
	    LinkNode( pb, b );

	    e = EdgeAlloc( pb, 1, b, 1 );
	    e->info = pb->imp->info;
	    LinkExport( pb, e );
	    LinkImport( b, e );

	    e = FindImport( c[idx]->F_BODY, ag->imp->isucc->isucc->eport );
	    UnlinkImport( e );
	    e->iport = 2;
	    LinkImport( b, e );

	    pb = b;
	    }

          for ( e = c[0]->exp; e != NULL; e = se ) {
	    se = e->esucc;

	    if ( e->eport == ag->exp->iport ) {
	      UnlinkExport( e );
	      e->eport = 1;
	      LinkExport( b, e );
	      }
            }

	  break;

        default:
	  Error2( "UnrollForalls", "ILLEGAL RETURN SUBGRAPH NODE" );
        }
      }


    /* FIX BODY REFERENCES TO CONTROL ROD */
    for ( idx = 0; idx < num; idx++, lo++ ) { /* CHANGES lo!!! */
      for ( e = c[idx]->F_BODY->exp; e != NULL; e = se ) {
	se = e->esucc;

	UnlinkExport( e );

	if ( (i = FindImport( c[0], e->eport )) == NULL ) {
	  /* REFERENCE TO CONTROL!!! */
          SPRINTF( s, "%d", lo );

          r = CopyString( s );

          e->CoNsT = r;
          e->eport = CONST_PORT;
          e->esucc = NULL;
          e->epred = NULL;
          e->src   = NULL;
	} else {
	  /* K PORT VALUE---NOT A CONSTANT (IsUnrollCandidate) */
	  e->eport = i->eport;
	  LinkExport( i->src, e );
	  }
	} 

      /* MOVE BODY NODES */
      pr = n->npred;
      for ( nn = c[idx]->F_BODY->G_NODES; nn != NULL; nn = sn ) {
	sn = nn->nsucc;
	UnlinkNode( nn );
	LinkNode( pr, nn );
	pr = nn;
	}
      }

    /* CLEAN THINGS UP */
    for ( i = n->imp; i != NULL; i = i->isucc )
      UnlinkExport( i );

    if ( n->exp != NULL )
      Error2( "UnrollForalls", "n->exp NOT NULL" );

    n = n->npred;

    UnlinkNode( n->nsucc );

    /* for ( idx = 0; idx < num; idx++ ) */
      /* free( c[idx] ); */

    ucnt++;
    }
}


void If1Unroll()
{
  register PNODE f;

  if ( maxunroll > MAX_UNROLL )
    maxunroll = MAX_UNROLL;

  for ( f = glstop->gsucc; f != NULL; f = f->gsucc ) {
    UnrollForalls( f );
    }
}
