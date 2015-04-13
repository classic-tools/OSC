/* if1check.c,v
 * Revision 12.7  1992/11/04  22:04:56  miller
 * Initial revision
 *
 * Revision 12.7  1992/10/21  18:08:30  miller
 * Initial RCS Version by Cann
 * */

#include "world.h"


/**************************************************************************/
/* GLOBAL **************    OptRemoveDeadNode      ************************/
/**************************************************************************/
void OptRemoveDeadNode( n )
PNODE n;
{
  /* Just a stub for the util.c module */
}

/**************************************************************************/
/* GLOBAL **************     OptNormalizeNode      ************************/
/**************************************************************************/
void OptNormalizeNode( n )
PNODE n;
{
  /* Just a stub for the util.c module */
}

/**************************************************************************/
/* LOCAL  **************         CheckError        ************************/
/**************************************************************************/
/* PURPOSE: PRINT MESSAGE msg AND LINE NUMBER ln TO output.               */
/**************************************************************************/

static void CheckError( ln, msg )
int   ln;
char *msg;
{
    FPRINTF( output, "%s: (line %d) %s\n", program, ln, msg );
}


/**************************************************************************/
/* LOCAL  **************    CheckIsRetNormalized   ************************/
/**************************************************************************/
/* PURPOSE: RETURN TRUE IF THE RETURN NODE OF LOOP l IS NORMALIZED:       */
/*          CONTAINS ONLY RETURN NODES. ALSO CHECK IF ReduceCatenate      */
/*          NODES HAVE BEEN CONVERTED.                                    */
/**************************************************************************/

static int CheckIsRetNormalized( l )
PNODE l;
{
    register PNODE n;

    if ( IsForall( l ) )
	n = l->F_RET->G_NODES;
    else
	n = l->L_RET->G_NODES;

    for ( /* EMPTY */; n != NULL; n = n->nsucc ) {
	if ( !IsReturn( n ) )
	    return( FALSE );

	switch ( n->type ) {
	    case IFReduce:
	    case IFRedLeft:
	    case IFRedRight:
	    case IFRedTree:
		if ( n->imp->CoNsT[0] == REDUCE_CATENATE )
                    if ( !IsConst( n->imp->isucc ) )
		        CheckError( n->if1line, "ReduceCatenate Node 1.0" );

		break;

	    default:
		break;
	    }
	}

    return( TRUE );
}


/**************************************************************************/
/* LOCAL  **************      IsGenNormalized        **********************/
/**************************************************************************/
/* PURPOSE: RETURN TRUE IF THE GENERATOR SUBGRAPH OF FORALL NODE f IS     */
/*          NORMALIZED: CONTAINS ONLY SCATTER AND RANGEGENERATE NODES.    */
/**************************************************************************/

static int IsGenNormalized( f )
PNODE f;
{
    register PNODE n;

    for ( n = f->F_GEN->G_NODES; n != NULL; n = n->nsucc )
	if ( !IsGenerate( n ) )
	    return( FALSE );

    return( TRUE );
}


/**************************************************************************/
/* LOCAL  **************      IsTestNormalized       **********************/
/**************************************************************************/
/* PURPOSE: RETURN TRUE IF THE TEST SUBGRAPH OF LOOPA OR LOOPB NODE l IS  */
/*          NORMALIZED: CONTAINS ONLY BOOLEAN OPERATION NODES.            */
/**************************************************************************/

static int IsTestNormalized( l )
PNODE l;
{
    register PNODE n;

    for ( n = l->L_TEST->G_NODES; n != NULL; n = n->nsucc )
	switch ( n->type ) {
	    case IFNot:
	    case IFLess:
	    case IFLessEqual:
	    case IFEqual:
	    case IFNotEqual:
                break;

	    case IFPlus:
	    case IFTimes:
	        if ( IsBoolean( n->exp->info ) )
		    break;

	    default:
		return( FALSE );
	    }

    return( TRUE );
}


/**************************************************************************/
/* LOCAL  **************        FloatType          ************************/
/**************************************************************************/
/* PURPOSE: RETURN THE FLOATING POINT TYPE OF CONSTANT c: 'd' FOR DOUBLES,*/
/*          'e' FOR REALS, AND ' ' FOR UNKNOWNS.                          */
/**************************************************************************/

static char FloatType( c )
register char *c;
{
    while ( *c != '\0' ) {
	if ( (*c == 'd') || (*c == 'D') )
	    return( 'd' );

	if ( (*c == 'e') || (*c == 'E') )
	    return( 'e' );
        
	c++;
	}

    return( ' ' );
}


/**************************************************************************/
/* LOCAL  **************   CheckConstantImports    ************************/
/**************************************************************************/
/* PURPOSE: CHECK THE IMPORTS OF NODE n FOR ILLEGAL FLOATING CONSTANTS,   */
/*          AND SIGNED ARITHMETIC CONSTANTS.  IF ALL OF n'S IMPORTS ARE   */
/*          CONSTANTS, A MESSAGE IS PRINTED.                              */
/**************************************************************************/

static void CheckConstantImports( n )
PNODE n;
{
    register PEDGE i;

    for ( i = n->imp; i != NULL; i = i->isucc ) {
	if ( !IsConst( i ) || i->CoNsT == NULL )
            continue;

	switch ( i->info->type ) {
            case IF_DOUBLE:
	        if ( FloatType( i->CoNsT ) == 'e' )
		    CheckError( i->if1line, "CONSTANT FORMAT ERROR" );

		if ( i->CoNsT[0] == '-' )
	            CheckError( i->if1line, "SIGNED DOUBLE CONSTANT" );

		break;

            case IF_REAL:
		if ( FloatType( i->CoNsT ) == 'd' )
	            CheckError( i->if1line, "CONSTANT FORMAT ERROR" );
		    
		if ( i->CoNsT[0] == '-' )
	            CheckError( i->if1line, "SIGNED REAL CONSTANT" );

		break;

            case IF_INTEGER:
		if ( i->CoNsT[0] == '-' )
	            CheckError( i->if1line, "SIGNED INTEGER CONSTANT" );

		break;

	    case IF_ARRAY:
		CheckError( i->if1line, "STRING CONSTANT" );
		break;

            default:
		break;
            }
	}

    for ( i = n->imp; i != NULL; i = i->isucc )
	if ( (!IsConst( i )) || (i->CoNsT == NULL) )
            break;

    if ( (i == NULL) &&  (!IsReturn( n )) && (!IsCall( n )) &&
	 (!IsGenerate( n )) && (IsSimple( n )))
	CheckError( n->if1line, "CONSTANT NODE NOT FOLDED" );
}


/**************************************************************************/
/* LOCAL  **************     AreKportsCombined     ************************/
/**************************************************************************/
/* PURPOSE: CHECK FOR REDUNDANT IMPORTS TO COMPOUND NODE c.               */
/**************************************************************************/

static int AreKportsCombined( c )
PNODE c;
{
    register PEDGE i1;
    register PEDGE i2;

    for ( i1 = c->imp; i1 != NULL; i1 = i1->isucc ) 
	for ( i2 = i1->isucc; i2 != NULL; i2 = i2->isucc ) 
	    if ( AreEdgesEqual( i1, i2 ) )
		return( FALSE );

    return( TRUE );
}


/**************************************************************************/
/* LOCAL  **************        AreSameType        ************************/
/**************************************************************************/
/* PURPOSE: CHECK IF THE TYPES i1 AND i2 ARE THE SAME. MULTIPLE TYPES ARE */
/*          TRANSPARENT. NOTE, A COMPLETE CHECK OF COMPLEX TYPES IS NOT   */
/*          DONE.                                                         */
/**************************************************************************/

static int AreSameType( i1, i2 )
PINFO i1;
PINFO i2;
{
    if ( IsMultiple( i1 ) )
	i1 = i1->A_ELEM;

    if ( IsMultiple( i2 ) )
	i2 = i2->A_ELEM;

    return( i1->type == i2->type );
}


/**************************************************************************/
/* LOCAL  **************      CheckIfDFOrdered     ************************/
/**************************************************************************/
/* PURPOSE: CHECK IF THE NODES IN THE NODE LIST OF GRAPH g ARE DFOrdered: */
/*          IF NODE b DEPENDS ON A VALUE GENERATED BY NODE a THEN THE     */
/*          LABEL(b) > LABEL(a).                                          */
/**************************************************************************/

static void CheckIfDFOrdered( g )
PNODE g;
{
    register PNODE n;
    register PEDGE i;

    for ( n = g->G_NODES; n != NULL; n = n->nsucc )
	if ( n->nsucc != NULL )
	    if ( n->nsucc->label <= n->label )
		CheckError( n->nsucc->if1line, "NODE NOT DFOrdered" );

    for ( n = g->G_NODES; n != NULL; n = n->nsucc )
	for ( i = n->imp; i != NULL; i = i->isucc )
	    if ( !IsConst( i ) )
		if ( i->src->label >= n->label )
		    CheckError( n->if1line, "NODE NOT DFOrdered" );
}


/**************************************************************************/
/* LOCAL  **************      CheckPortNumbers     ************************/
/**************************************************************************/
/* PURPOSE: CHECK IF THE IMPORT PORT NUMBERS OF g'S IMPORTS ARE IN        */
/*          SEQUENCE BEGINNING WITH p.  THE NEXT EXPECTED PORT NUMBER IS  */
/*          RETURNED.  IF gg IS NOT NULL, THEN AN IMPORT IS ONLY CHECKED  */
/*          IF IT IS NOT ALSO AN IMPORT TO gg: ONLY IF IT IS A LOOPA T    */
/*          PORT NUMBER.                                                  */
/**************************************************************************/

static int CheckPortNumbers( p, g, gg )
int   p;
PNODE g;
PNODE gg;
{
    register PEDGE i;

    for ( i = g->imp; i != NULL; i = i->isucc ) {
	if ( gg != NULL )
	    if ( IsImport( gg, i->iport ) )
		continue;

        if ( i->iport != p++ )
	    CheckError( i->if1line, "ILLEGAL IMPORT PORT NUMBER" );
        }

    return( p );
}


/**************************************************************************/
/* LOCAL  **************           IsUsed          ************************/
/**************************************************************************/
/* PURPOSE: CHECK THAT ALL USAGES OF i->iport IN GRAPH NODE g ARE OF THE  */
/*          SAME TYPE. IF i->iport IS USED, TRUE IS RETURNED, ELSE FALSE. */
/**************************************************************************/

static int IsUsed( g, i )
PNODE g;
PEDGE i;
{
    register PEDGE e;
    register int   u = FALSE;

    for ( e = g->exp; e != NULL; e = e->esucc )
	if ( e->eport == i->iport ) {
	    u = TRUE;

	    if ( !AreSameType( e->info, i->info ) ) {
		CheckError( i->if1line, "** USAGE TYPE MISMATCH" );
		CheckError( e->if1line, "   USAGE TYPE MISMATCH" );
		}
	    }

    return( u );
}


/**************************************************************************/
/* LOCAL  **************    CheckForUnusedKports   ************************/
/**************************************************************************/
/* PURPOSE: CHECK THAT ALL IMPORTS TO COMPOUND NODE c ARE USED.           */
/**************************************************************************/

static void CheckForUnusedKports( c )
PNODE c;
{
    register PEDGE i;
    register int   u;
    register PNODE g;

    for ( i = (IsTagCase(c))? c->imp->isucc : c->imp; i != NULL; i = i->isucc) {
	for ( u = FALSE, g = c->C_SUBS; g != NULL; g = g->gsucc )
            u |= IsUsed( g, i );

        if ( !u )
	    CheckError( i->if1line, "K IMPORT NOT USED" );
	}
}


/**************************************************************************/
/* LOCAL  **************   CheckForUnusedRports    ************************/
/**************************************************************************/
/* PURPOSE: CHECK THAT ALL IMPORTS OF SUBGRAPH g ARE EXPORTS OF THE       */
/*          COMPOUND NODE DEFINING g.                                     */
/**************************************************************************/

static void CheckForUnusedRports( g )
PNODE g;
{
    register PEDGE i;

    for ( i = g->imp; i != NULL; i = i->isucc )
        if ( !IsUsed( g->G_DAD, i ) )
	    CheckError( i->if1line, "R EXPORT NOT USED" );
}


/**************************************************************************/
/* LOCAL  **************    CheckForUnusedTports   ************************/
/**************************************************************************/
/* PURPOSE: CHECK THAT ALL IMPORTS TO FORALL f'S BODY SUBGRAPH ARE USED.  */
/**************************************************************************/

static void CheckForUnusedTports( f )
PNODE f;
{
    register PEDGE i;

    for ( i = f->F_BODY->imp; i != NULL; i = i->isucc )
        if ( !IsUsed( f->F_RET, i ) )
	    CheckError( i->if1line, "T IMPORT NOT USED" );
}


/**************************************************************************/
/* LOCAL  **************   CheckForUnusedLTports   ************************/
/**************************************************************************/
/* PURPOSE: CHECK THAT THE T IMPORTS OF LOOPA l'S BODY ARE USED AND CHECK */
/*          THAT THE L IMPORTS OF LOOP l'S INITIAL SUBGRAPH ARE USED.     */
/**************************************************************************/

static void CheckForUnusedLTports( l )
PNODE l;
{
    register PEDGE i;

    for ( i = l->L_BODY->imp; i != NULL; i = i->isucc ) {
	if ( IsUsed( l->L_TEST, i ) || 
             IsUsed( l->L_BODY, i ) ||
             IsUsed( l->L_RET,  i )  )
            continue;

	CheckError( i->if1line, "T IMPORT NOT USED" );
	}

    for ( i = l->L_INIT->imp; i != NULL; i = i->isucc ) {
	if ( IsUsed( l->L_TEST, i ) || 
             IsUsed( l->L_BODY, i ) ||
             IsUsed( l->L_RET,  i )   )
            continue;

	CheckError( i->if1line, "L IMPORT NOT USED" );
        }
}


/**************************************************************************/
/* LOCAL  **************    CheckForUnnecEdges     ************************/
/**************************************************************************/
/* PURPOSE: CHECK FOR UNNECESSARY EDGES AND LITERALS IN THE SUBGRAPHS OF  */
/*          LOOP l. THE FOLLOWING ARE CONSIDERED UNNECESSARY:             */
/*                                                                        */
/*          1. "T := CoNsTant, M, OR K" (FORALL BODY)                     */
/*          2. "L := CoNsTant OR K", L NOT REDEFINED IN BODY              */
/*          3. "L := OLD L"                                               */
/*          4. "T := CoNsTant, K, OR NOT REDEFINED L (LOOPA BODY)"        */
/*          5. "L := CoNsTant OR K", "L := SAME CoNsTant OR K"            */
/**************************************************************************/

static void CheckForUnnecEdges( l )
PNODE l;
{
    register PEDGE i;
    register PEDGE ii;

    switch ( l->type ) {
        case IFForall:
	    for ( i = l->F_BODY->imp; i != NULL; i = i->isucc )
		if ( IsConst( i ) )
		    CheckError( i->if1line, "UNNECESSARY LITERAL: T := CoNsT" );
		else if ( IsSGraph( i->src ) )
		    CheckError( i->if1line, "UNNECESSARY EDGE: T := M OR K" );

	    break;

        case IFLoopA:
	case IFLoopB:
	    for ( i = l->L_BODY->imp; i != NULL; i = i->isucc ) {
		if ( IsConst( i ) )
		    continue;

		if ( IsSGraph( i->src ) && ( i->iport == i->eport ) )
	            CheckError( i->if1line, "UNNECESSARY EDGE: L := OLD L" );
                }

	    for ( i = l->L_INIT->imp; i != NULL; i = i->isucc ) {
		if ( IsImport( l->L_BODY, i->iport ) )
		    continue;

		if ( IsConst( i ) )
                    CheckError( i->if1line, "UNNECESSARY EDGE: L NR := CoNsT" );
                else if ( IsSGraph( i->src ) )
	            CheckError( i->if1line, "UNNECESSARY EDGE: L NR := K" );
                }

	    for ( i = l->L_INIT->imp; i != NULL; i = i->isucc ) {
		if ( (ii = FindImport( l->L_BODY, i->iport ) ) == NULL )
		    continue;

                if ( IsConst( i ) ) {
		    if ( !IsConst( ii ) )
			continue;

                    if ( !AreConstsEqual( i, ii ) )
			continue;

	            CheckError( i->if1line, 
				"UNNECESSARY EDGES: L := CoNsT1, L := CoNsT1" );
		    }
                else if ( IsSGraph( i->src ) ) {
		    if ( !IsSGraph( ii->src ) )
			continue;

                    if ( i->eport != ii->eport )
			continue;

	            CheckError( i->if1line, 
				"UNNECESSARY EDGE: L := K1, L := K1" );
		    }
                }

            if ( !IsLoopA( l ) )
		break;

            for ( i = l->L_BODY->imp; i != NULL; i = i->isucc ) {
		if ( IsImport( l->L_INIT, i->iport ) )
	            continue;

	        if ( IsConst( i ) )
	            CheckError( i->if1line, "UNNECESSARY EDGE: T := CoNsT" );
                else if ( IsSGraph( i->src ) )
	            if ( !IsImport( l->L_BODY, i->eport ) )
			    CheckError( i->if1line, 
					"UNNECESSARY EDGE: T := K OR L NR" );
                }

	    break;

	default:
	    break;
        }
}


/**************************************************************************/
/* LOCAL  **************     CheckCompoundNode     ************************/
/**************************************************************************/
/* PURPOSE: FOR COMPOUND NODE c, CHECK THAT ALL M, T, L, K, AND R PORT    */
/*          VALUES ARE USED; CHECK FOR UNNECESSARY EDGES AND LITERALS;    */
/*          CHECK THAT ALL PORT NUMBERS ARE CORRECTLY ASSIGNED; CHECK     */
/*          THAT TEST, INITIAL, AND RETURN SUBGRAPHS ARE NORMALIZED; AND  */
/*          MAKE SURE ALL REFERENCED SUBGRAPH EXPORTS ARE DEFINED.        */
/**************************************************************************/

static void CheckCompoundNode( c )
PNODE c;
{
    register PNODE g;
    register int   p;
    register PEDGE e;
    register PNODE n;

    CheckForUnnecEdges( c );

    switch( c->type ) {
	case IFSelect:
            if ( IsConst( c->S_TEST->imp ) )
                CheckError( c->if1line, "UNREACHABLE IF BRANCH" );

	    if ( !IsNodeListEmpty( c->S_TEST ) )
		CheckError( c->S_TEST->if1line, "SELECT TEST NOT NORMALIZED" );

            CheckForUnusedRports( c->S_ALT  );
            CheckForUnusedRports( c->S_CONS );

            CheckPortNumbers( 1, c, NULL_NODE ); /* K */

	    for ( g = c->C_SUBS; g != NULL; g = g->gsucc )
		CheckPortNumbers( 1, g, NULL_NODE ); /* R */

            for ( g = c->C_SUBS; g != NULL; g = g->gsucc )
		for ( e = g->exp; e != NULL; e = e->esucc )
		    if ( !IsImport( c, e->eport ) )
			CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

	    break;

        case IFTagCase:
            CheckPortNumbers( 1, c, NULL_NODE ); /* K */

	    for ( g = c->C_SUBS; g != NULL; g = g->gsucc ) {
		CheckForUnusedRports( g );
		CheckPortNumbers( 1, g, NULL_NODE );                          /* R */
		}

            for ( g = c->C_SUBS; g != NULL; g = g->gsucc )
		for ( e = g->exp; e != NULL; e = e->esucc )
		    if ( !IsImport( c, e->eport ) )
			CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

	    break;

        case IFForall:
	    if ( !IsGenNormalized( c ) )
		CheckError( c->F_GEN->if1line, "FORALL GEN NOT NORMALIZED" );

	    if ( !CheckIsRetNormalized( c ) )
		CheckError( c->F_GEN->if1line, "FORALL RETURN NOT NORMALIZED" );

	    CheckForUnusedRports( c->F_RET );
	    CheckForUnusedTports( c );

	    p = CheckPortNumbers( 1, c, NULL_NODE ); /* K */
	    p = CheckPortNumbers( p, c->F_GEN, NULL_NODE ); /* M */

	    CheckPortNumbers( p, c->F_BODY, NULL_NODE ); /* T */
	    CheckPortNumbers( 1, c->F_RET,  NULL_NODE ); /* R */

            for ( e = c->F_GEN->exp; e != NULL; e = e->esucc )
		if ( !IsImport( c, e->eport ) )
	            CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

            for ( e = c->F_BODY->exp; e != NULL; e = e->esucc )
		if ( (!IsImport( c,        e->eport )) && 
		     (!IsImport( c->F_GEN, e->eport ))   )
	            CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

            for ( e = c->F_RET->exp; e != NULL; e = e->esucc )
		if ( (!IsImport( c,         e->eport )) &&
		     (!IsImport( c->F_GEN,  e->eport )) &&
		     (!IsImport( c->F_BODY, e->eport ))   )
	            CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

	    for ( n = c->F_BODY->G_NODES; n != NULL; n = n->nsucc )
		if ( OptIsInvariant( n ) )
		    CheckError( n->if1line, "LOOP INVARIANT" );

	    break;

        case IFLoopA:
	case IFLoopB:
	    if ( !IsNodeListEmpty( c->L_INIT ) )
		CheckError( c->L_INIT->if1line, "LOOP INIT NOT NORMALIZED" );

	    if ( !IsTestNormalized( c ) )
		CheckError( c->L_TEST->if1line, "LOOP TEST NOT NORMALIZED" );

	    if ( !CheckIsRetNormalized( c ) )
		CheckError( c->L_RET->if1line, "LOOP RETURN NOT NORMALIZED" );

	    CheckForUnusedRports( c->L_RET );
	    CheckForUnusedLTports( c );

	    p = CheckPortNumbers( 1, c, NULL_NODE );	/* K */
	    p = CheckPortNumbers( p, c->L_INIT, NULL_NODE );	/* L */

	    if ( IsLoopA( c ) )
		CheckPortNumbers( p, c->L_BODY, c->L_INIT ); /* T */

            CheckPortNumbers( 1, c->L_RET, NULL_NODE ); /* R */

            for ( e = c->L_INIT->exp; e != NULL; e = e->esucc )
		if ( !IsImport( c, e->eport ) )
	            CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

            for ( e = c->L_BODY->exp; e != NULL; e = e->esucc )
		if ( (!IsImport( c,         e->eport )) &&
		     (!IsImport( c->L_INIT, e->eport ))   )
	            CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

            for ( e = c->L_TEST->exp; e != NULL; e = e->esucc )
		if ( (!IsImport( c,         e->eport )) &&
		     (!IsImport( c->L_INIT, e->eport )) &&
		     (!IsImport( c->L_BODY, e->eport ))   )
	            CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

            for ( e = c->L_RET->exp; e != NULL; e = e->esucc )
		if ( (!IsImport( c,         e->eport )) &&
		     (!IsImport( c->L_INIT, e->eport ))   )
	            CheckError( e->if1line, "SUBGRAPH EXPORT NOT DEFINED" );

	    for ( n = c->L_BODY->G_NODES; n != NULL; n = n->nsucc )
		if ( OptIsInvariant( n ) )
		    CheckError( n->if1line, "LOOP INVARIANT" );

	    break;
	}

    CheckForUnusedKports( c );
}


/**************************************************************************/
/* LOCAL  **************         CheckNode         ************************/
/**************************************************************************/
/* PURPOSE: CHECK THE STRUCTURE AND SEMANTICS OF ALL NODES IN GRAPH g.    */
/*          BESIDES THE CHECKS PERFORMED BY CheckCompoundNode, DATA FLOW  */
/*          ORDERING IS CHECKED; CONSTANT PROPAGATION IS CHECKED; A       */
/*          CHECK IS MADE FOR REDUNDANT NOT PAIRS; A CHECK IS MADE FOR    */
/*          REDUNDANT IMPORTS TO ALL COMPOUND NODES; A CHECK IS MADE FOR  */
/*          UNDEFINED AND DEAD NODES; THE FORMAT OF ALL CONSTANT IMPORTS  */
/*          IS CHECKED; AND STREAM LIMIT LOW NODES ARE IDENTIFIED.        */
/**************************************************************************/

static void CheckNode( g )
register PNODE g;
{
    register PEDGE i;
    register PEDGE e;
    register PNODE n;

    if ( IsIGraph( g ) )
	return;

    CheckIfDFOrdered( g );
    CheckConstantImports( g );

    for ( n = g->G_NODES; n != NULL; n = n->nsucc ) {
	CheckConstantImports( n );

	if ( IsALimL( n ) )
	    if ( IsStream( n->imp->info ) )
	        CheckError( n->if1line, "STREAM LIMIT LOW" );

	if ( n->exp == NULL )
	    CheckError( n->if1line, "DEAD NODE" );

        for ( e = n->exp; e != NULL; e = e->esucc )
	    if ( IsUndefined( e->dst ) )
		CheckError( e->if1line, "DESTINATION NODE UNDEFINED" );

        for ( i = n->imp; i != NULL; i = i->isucc )
	    if ( IsConst( i ) )
		continue;
            else if ( IsUndefined( i->src ) )
		CheckError( i->if1line, "SOURCE NODE UNDEFINED" );

        if ( IsCompound( n ) ) {
	    if ( !AreKportsCombined( n ) )
		CheckError( n->if1line, "REDUNDANT K IMPORTS" );

	    for ( i = n->imp; i != NULL; i = i->isucc )
		if ( IsConst( i ) )
		    CheckError( i->if1line, "CONSTANT NOT PROPAGATED" );

	    for ( g = n->C_SUBS; g != NULL; g = g->gsucc )
		CheckNode( g );

	    CheckCompoundNode( n );
	    continue;
	    }

	if ( IsNot( n ) )
	    if ( (n->exp->esucc == NULL) && (IsNot( n->exp->dst )) )
		CheckError( n->if1line, "REDUNDANT NOT NODE PAIRS" ); 
        }
}


/**************************************************************************/
/* GLOBAL **************          If1Check         ************************/
/**************************************************************************/
/* PURPOSE: CHECK THE STRUCTURE AND SEMANTICS OF ALL FUNCTION GRAPHS.     */
/**************************************************************************/

void If1Check()
{
    register PNODE f;

    for ( f = glstop->gsucc; f != NULL; f = f->gsucc )
	CheckNode( cfunct = f );
}
