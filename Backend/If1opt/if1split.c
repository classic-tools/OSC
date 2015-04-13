/* if1split.c,v
 * Revision 12.7  1992/11/04  22:04:58  miller
 * Initial revision
 *
 * Revision 12.7  1992/10/21  18:08:37  miller
 * Initial RCS Version by Cann
 * */

/* static PEDGE k; */

#include "world.h"


static PEDGE c;                              /* FORALL NODE'S CONTROL ROD */
static PEDGE lo;                             /* FORALL NODE'S LOWER BOUND */
static PEDGE hi;                             /* FORALL NODE'S UPPER BOUND */


static int   lcnt  = 0;                      /* COUNT OF LOW SPLITS       */
static int  nlcnt  = 0;                      /* COUNT OF NOT_LOW SPLITS   */
static int   hcnt  = 0;                      /* COUNT OF HIGH SPLITS      */
static int  nhcnt  = 0;                      /* COUNT OF NOT_HIGH SPLITS  */
static int  rfail  = 0;                      /* COUNT RETURN GRAPH ABORTS */


/**************************************************************************/
/* LOCAL  **************       GetOperand          ************************/
/**************************************************************************/
/* PURPOSE: FIND THE SOURCE OF EDGE e IN FORALL NODE f. THIS ROUTINE IS   */
/*          SPECIFIC TO LOOP SPLITTING, AND SHOULD NOT BE USED OUTSIDE    */ 
/*          THIS CONTEXT.                                                 */
/**************************************************************************/

static PEDGE GetOperand( f, e )
PEDGE e;
PNODE f;
{
  register PEDGE i1;

  if ( IsConst( e ) )
    return( e );

  if ( !IsSGraph( e->src ) )
    return( NULL );

  i1 = FindImport( f, e->eport );

  /* IS e A REFERENCE TO f'S CONTROL ROD? */
  if ( i1 == NULL ) {
    i1 = FindImport( f->F_GEN, e->eport );

    if ( i1 == NULL )
      return( NULL );
    }
  
  return( i1 );
}


/**************************************************************************/
/* LOCAL  **************         SplitType         ************************/
/**************************************************************************/
/* PURPOSE: IDENTIFY THE SPLIT TYPE OF FORALL NODE f GIVEN NODE n.        */
/**************************************************************************/

static int SplitType( f, n )
PNODE f;
PNODE n;
{
   register PEDGE i1;
   register PEDGE i2;

   switch ( n->type ) {
      case IFNotEqual:
	/* GET THE LEFT OPERAND OF THE EQUAL NODE */
	if ( (i1 = GetOperand( f, n->imp )) == NULL ) 
	  return( BAD );

	/* GET THE RIGHT OPERAND OF THE EQUAL NODE */
	if ( (i2 = GetOperand( f, n->imp->isucc )) == NULL ) 
	  return( BAD );

	/* FOR i in L,H ... */

	if ( i1 == f->F_GEN->imp ) {
	  /* i ~= L */
	  if ( AreValuesEqual( i2, lo ) )
	    return( NOT_LOW );

	  /* i ~= H */
	  if ( AreValuesEqual( i2, hi ) )
	    return( NOT_HIGH );
	  }

	return( BAD );

      case IFEqual:
	/* GET THE LEFT OPERAND OF THE EQUAL NODE */
	if ( (i1 = GetOperand( f, n->imp )) == NULL ) 
	  return( BAD );

	/* GET THE RIGHT OPERAND OF THE EQUAL NODE */
	if ( (i2 = GetOperand( f, n->imp->isucc )) == NULL ) 
	  return( BAD );

	/* FOR i in L,H ... */

	if ( i1 == f->F_GEN->imp ) {
	  /* i = L */
	  if ( AreValuesEqual( i2, lo ) )
	    return( LOW );

	  /* i = H */
	  if ( AreValuesEqual( i2, hi ) )
	    return( HIGH );
	  }

	return( BAD );

      default:
	return( BAD );
      }
}


/**************************************************************************/
/* LOCAL  **************        FixBody            ************************/
/**************************************************************************/
/* PURPOSE: ADJUST THE t CONDITIONALS IN FORALL f TO YIELD v.             */
/**************************************************************************/

static void FixBody( t, f, v )
int    t;
PNODE  f;
char  *v;
{
  register PNODE n;
  register PEDGE e;
  register PEDGE se;
  register int   tt;
  register PNODE sn;

  for ( n = f->F_BODY->G_NODES; n != NULL; n = sn ) {
    sn = n->nsucc;

    /* SHORT CIRCUIT FOR FASTER EXECUTION */
    if ( !(n->type == IFEqual || n->type == IFNotEqual) )
      continue;

    if ( (tt = SplitType( f, n )) == BAD )
      continue;

    if ( t != tt )
      continue;

    for ( e = n->exp; e != NULL; e = se ) {
      se = e->esucc;

      UnlinkExport( e );
      e->src   = NULL;
      e->eport = CONST_PORT;
      e->CoNsT = CopyString( v );
      }

    n->exp = NULL;
    OptRemoveDeadNode( n );
    }
}


/* kk = hi or lo, ee = bound edge, rg = range generate, v = value */

/**************************************************************************/
/* LOCAL  **************         FixControl2       ************************/
/**************************************************************************/
/* PURPOSE: CHANGE ee (AN IMPORT TO RangeGenerate NODE rg) TO REFERENCE   */
/*          THE RESULT OF ADDING kk (AN IMPORT VALUE TO rg) TO v.         */
/**************************************************************************/

static void FixControl2( kk, ee, rg, v )
PEDGE   kk;
PEDGE   ee;
PNODE   rg;
char   *v;
{
  register PNODE f;
  register PEDGE e;
  register int   iport;
  register PNODE op;

  f = rg->exp->dst->G_DAD;

  op = NodeAlloc( ++maxint, IFPlus );
  CopyVitals( f, op );
  LinkNode( f->npred, op );

  e = EdgeAlloc( op, 1, f, ++maxint );
  iport = maxint;
  e->info = integer;
  LinkImport( f, e );
  LinkExport( op, e );

  e = CopyEdge( kk, kk->src, op );
  e->iport = 1;
  LinkImport( op, e );
  if ( !IsConst( kk ) )
    LinkExport( kk->src, e ); 

  e = EdgeAlloc( NULL_NODE, CONST_PORT, op, 2 );
  e->info = integer;
  e->CoNsT = CopyString( v );
  LinkImport( op, e );

  if ( IsConst( ee ) ) {
    ee->eport = iport;
    ee->CoNsT = NULL;
  } else {
    UnlinkExport( ee );
    ee->eport = iport;
    }
  
  LinkExport( rg->exp->dst, ee );
}


/* ee = RangeGenerate import, rg = RangeGenerate, mt = type */

/**************************************************************************/
/* LOCAL  **************         FixControl1       ************************/
/**************************************************************************/
/* PURPOSE: CHANGE ee (AN IMPORT TO RangeGenerate NODE rg) TO REFERENCE   */
/*          THE RESULT OF APPLYING mt (MIN OR MAX) TO THE IMPORT VALUES   */
/*          OF rg (GLOBALS lo AND hi).                                    */
/**************************************************************************/

static void FixControl1( ee, rg, mt )
PEDGE   ee;
PNODE   rg;
int     mt;
{
  register PNODE f;
  register PEDGE e;
  register int   iport;
  register PNODE min;

  f = rg->exp->dst->G_DAD;

  min = NodeAlloc( ++maxint, mt );
  CopyVitals( f, min );
  LinkNode( f->npred, min );

  e = EdgeAlloc( min, 1, f, ++maxint );
  iport = maxint;
  e->info = integer;
  LinkImport( f, e );
  LinkExport( min, e );

  e = CopyEdge( lo, lo->src, min );
  e->iport = 1;
  LinkImport( min, e );
  if ( !IsConst( lo ) )
    LinkExport( lo->src, e ); 

  e = CopyEdge( hi, hi->src, min );
  e->iport = 2;
  LinkImport( min, e );
  if ( !IsConst( hi ) )
    LinkExport( hi->src, e ); 

  if ( IsConst( ee ) ) {
    ee->eport = iport;
    ee->CoNsT = NULL;
  } else {
    UnlinkExport( ee );
    ee->eport = iport;
    }

  LinkExport( rg->exp->dst, ee );
}


/**************************************************************************/
/* GLOBAL **************      WriteSplitInfo       ************************/
/**************************************************************************/
/* PURPOSE: WRITE LOOP SPLITTING INFORMATION TO stderr.                   */
/**************************************************************************/

void WriteSplitInfo()
{
  FPRINTF( stderr, "\n   * LOOP SPLITTING\n\n" );
  FPRINTF( stderr, " Low Splits:               %d\n", lcnt  );
  FPRINTF( stderr, " High Splits:              %d\n", hcnt  );
  FPRINTF( stderr, " Not Low Splits:           %d\n", nlcnt );
  FPRINTF( stderr, " Not High Splits:          %d\n", nhcnt );
  FPRINTF( stderr, " Return Subgraph Failures: %d\n", rfail );
}


/**************************************************************************/
/* LOCAL  **************     IsSplitCandidate      ************************/
/**************************************************************************/
/* PURPOSE: CHECK IF FORALL NODE f IS A LOOP SPLIT CANDIDATE. IF SO, THEN */
/*          RETURN ITS SPLIT TYPE (LOW, NOT_LOW, HIGH, NOT_HIGH); ELSE    */
/*          RETURN BAD.                                                   */
/**************************************************************************/

static int IsSplitCandidate( f )
PNODE f;
{
  register PNODE r;
  register PNODE n;
  register int   s;
  register int   t;

  r = f->F_GEN->G_NODES;

  if ( r->nsucc != NULL )
    return( BAD );

  if ( r->type != IFRangeGenerate )
    return( BAD );

  /* ONLY SPLIT INNERMOST LOOPS!!! */
  if ( !IsInnerLoop( f->F_BODY ) ) {
    return( BAD );
    }

  /* GET THE CONTROL ROD */
  c = r->exp;

  /* GET THE LOW AND HIGH BOUND OF THE LOOP */
  if ( IsConst( r->imp ) )
    lo = r->imp;
  else
    lo = FindImport( f, r->imp->eport );

  if ( lo == NULL )
    return( BAD );

  if ( IsConst( r->imp->isucc ) )
    hi = r->imp->isucc;
  else
    hi = FindImport( f, r->imp->isucc->eport );

  if ( hi == NULL )
    return( BAD );

  if ( AreValuesEqual( lo, hi ) )
    return( BAD );

  /* IDENTIFY THE SPLIT TYPE, IF SUCH A TYPE EVEN EXISTS */
  s = BAD;

  for ( n = f->F_BODY; n != NULL; n = n->nsucc ) {
    /* WITH SHORT CIRCUIT FOR FASTER EXECUTION */
    if ( n->type == IFEqual || n->type == IFNotEqual )
      if ( (t = SplitType( f, n )) != BAD )
        s = t;
    }

  if ( s == BAD )
    return( BAD );

  /* CURRENTLY, SPLIT LOOPS MUST BUILD UNFILTERED ARRAYS */
  for ( n = f->F_RET->G_NODES; n != NULL; n = n->nsucc )
    switch ( n->type ) {
      case IFAGather:
	/* FILTER? */
	if ( n->imp->isucc->isucc != NULL )
	  return( BAD );

	break;

      default:
	rfail++;
	return( BAD );
      }

  return( s );
}


/**************************************************************************/
/* LOCAL  **************         DoHighSplit       ************************/
/**************************************************************************/
/* PURPOSE: THE SPLIT TYPE OF FORALL f1 IS ONE OF THE HIGH TYPES (DEFINED */
/*          BY kind).  THIS ROUTINE SPLITS f1 INTO TWO LOOPS, THE SECOND  */
/*          JUST HANDLES THE HIGH TEST.                                   */
/**************************************************************************/

static void DoHighSplit( f1, kind )
PNODE f1;
int   kind;
{
  register PNODE f2;
  register PNODE cat;
  register PEDGE e;
  register PEDGE i;
  register PEDGE se;
  register PEDGE e1;
  register PEDGE e2;
  register PNODE ag;

  f2 = CopyNode( f1 );
  LinkNode( f1, f2 );

  for ( i = f1->imp; i != NULL; i = i->isucc ) {
    e = CopyEdge( i, i->src, f2 );

    LinkImport( f2, e );

    if ( !IsConst( i ) )
      LinkExport( i->src, e );
    }

  for ( ag = f1->F_RET->G_NODES; ag != NULL; ag = ag->nsucc ) {
    cat = NodeAlloc( ++maxint, IFACatenate );
    CopyVitals( f2, cat );
    LinkNode( f2, cat );

    for ( e = f1->exp; e != NULL; e = se ) {
      se = e->esucc;

      if ( e->eport == ag->exp->iport ) {
        UnlinkExport( e );
        e->eport = 1;
        LinkExport( cat, e );
        }
      }

    e1 = EdgeAlloc( f1, ag->exp->iport, cat, 1 );
    e1->info = ag->exp->info;
    LinkImport( cat, e1 );
    LinkExport( f1, e1 );

    e2 = EdgeAlloc( f2, ag->exp->iport, cat, 2 );
    e2->info = ag->exp->info;
    LinkImport( cat, e2 );
    LinkExport( f2, e2 );
    }

  if ( kind == HIGH ) {
    FixBody( HIGH, f1, "FALSE" );
    FixBody( HIGH, f2, "TRUE" );
    hcnt++;
  } else {
    FixBody( NOT_HIGH, f1, "TRUE" );
    FixBody( NOT_HIGH, f2, "FALSE" );
    nhcnt++;
    }

  FixControl1( f2->F_GEN->imp->src->imp, f2->F_GEN->imp->src, IFMax );
  FixControl2( hi, c->src->imp->isucc, c->src, "-1" );

  NewCompoundID(f2);		/* Give it a unique ID */
  f2->ThinCopy = TRUE;		/* TO DISABLE CONCURRENTIZATION OF THIN COPY */
}


/**************************************************************************/
/* LOCAL  **************          DoLowSplit       ************************/
/**************************************************************************/
/* PURPOSE: THE SPLIT TYPE OF FORALL f1 IS ONE OF THE LOW TYPES (DEFINED  */
/*          BY kind).  THIS ROUTINE SPLITS f1 INTO TWO LOOPS, THE FIRST   */
/*          JUST HANDLES THE LOW TEST.                                    */
/**************************************************************************/

static void DoLowSplit( f1, kind )
PNODE f1;
int   kind;
{
  register PNODE f2;
  register PNODE cat;
  register PEDGE e;
  register PEDGE i;
  register PEDGE se;
  register PEDGE e1;
  register PEDGE e2;
  register PNODE ag;

  f2 = CopyNode( f1 );
  LinkNode( f1, f2 );

  for ( i = f1->imp; i != NULL; i = i->isucc ) {
    e = CopyEdge( i, i->src, f2 );

    LinkImport( f2, e );

    if ( !IsConst( i ) )
      LinkExport( i->src, e );
    }

  for ( ag = f1->F_RET->G_NODES; ag != NULL; ag = ag->nsucc ) {
    cat = NodeAlloc( ++maxint, IFACatenate );
    CopyVitals( f2, cat );
    LinkNode( f2, cat );

    for ( e = f1->exp; e != NULL; e = se ) {
      se = e->esucc;

      if ( e->eport == ag->exp->iport ) {
        UnlinkExport( e );
        e->eport = 1;
        LinkExport( cat, e );
        }
      }

    e1 = EdgeAlloc( f1, ag->exp->iport, cat, 1 );
    e1->info = ag->exp->info;
    LinkImport( cat, e1 );
    LinkExport( f1, e1 );

    e2 = EdgeAlloc( f2, ag->exp->iport, cat, 2 );
    e2->info = ag->exp->info;
    LinkImport( cat, e2 );
    LinkExport( f2, e2 );
    }

  if ( kind == LOW ) {
    FixBody( LOW, f1, "TRUE" );
    FixBody( LOW, f2, "FALSE" );
    lcnt++;
  } else {
    FixBody( NOT_LOW, f1, "FALSE" );
    FixBody( NOT_LOW, f2, "TRUE" );
    nlcnt++;
    }

  FixControl2( lo, f2->F_GEN->imp->src->imp, f2->F_GEN->imp->src, "1" );
  FixControl1( c->src->imp->isucc, c->src, IFMin );

  NewCompoundID(f1);		/* Give it a unique ID */
  f1->ThinCopy = TRUE;		/* TO DISABLE CONCURRENTIZATION OF THIN COPY */
}


/**************************************************************************/
/* LOCAL  **************       SplitForalls        ************************/
/**************************************************************************/
/* PURPOSE: SPLIT THE FORALL LOOPS IN GRAPH g.                            */
/**************************************************************************/

static void SplitForalls( g )
PNODE g;
{
  register PNODE n;
  register PNODE sg;
  register int   kind;

  for ( n = g->G_NODES; n != NULL; n = n->nsucc ) {
    if ( IsCompound( n ) ) {
      PropagateConst( n );

      for ( sg = n->C_SUBS; sg != NULL; sg = sg->gsucc )
	SplitForalls( sg );
      }

    if ( !IsForall( n ) )
      continue;
    
    if ( (kind = IsSplitCandidate( n )) == BAD )
      continue;

    switch( kind ) {
      case NOT_LOW:
      case LOW:
        DoLowSplit( n, kind );
	break;

      case HIGH:
      case NOT_HIGH:
        DoHighSplit( n, kind );
	break;

      default:
	Error2( "SplitForalls", "ILLEGAL IsSplitCandidate KIND" );
      }
    }
}


/**************************************************************************/
/* GLOBAL **************         If1Split          ************************/
/**************************************************************************/
/* PURPOSE: DETECT AND ELIMINATE BOUNDARY CONDITION CONDITIONALS FROM     */
/*          THE FORALL NODES IN THE PROGRAM. FOR EXAMPLE:                 */
/*                                                                        */
/*    for i in 1,n x := if ( i == 1 ) then 1 else 0 end if;               */
/*    returns array of x end for   BECOMES                                */
/*                                                                        */
/*    array [1:1] || for i in 2,n x:= 0; returns array of x end for       */
/*                                                                        */
/**************************************************************************/

void If1Split()
{
  register PNODE f;

  for ( f = glstop->gsucc; f != NULL; f = f->gsucc )
    SplitForalls( f );
}
