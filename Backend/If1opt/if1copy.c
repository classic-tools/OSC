/* if1copy.c,v
 * Revision 12.7  1992/11/04  22:04:56  miller
 * Initial revision
 *
 * Revision 12.7  1992/10/21  18:08:31  miller
 * Initial RCS Version by Cann
 * */

#include "world.h"


/**************************************************************************/
/* GLOBAL **************     CopyEdgeAndReset      ************************/
/**************************************************************************/
/* PURPOSE: COPY EDGE e AND LINK INTO THE GRAPH, BUT USING src AND dst AS */
/*          THE SOURCE AND DESTINATION NODES.                             */
/**************************************************************************/

void CopyEdgeAndReset( e, src, dst )
PEDGE e;
PNODE src;
PNODE dst;
{
  register PEDGE ee;

  ee = CopyEdge( e, (IsConst(e))? NULL : src, dst );

  if ( !IsConst( e ) )
    LinkExport( src, ee );

  LinkImport( dst, ee );
}


/**************************************************************************/
/* GLOBAL **************        CopyExports        ************************/
/**************************************************************************/
/* PURPOSE: COPY THE EXPORT LIST OF NODE n1 AND ATTACH IT TO NODE n2. A   */
/*          COPIED EXPORT IS NOT LINKED TO ITS COPY'S  DESTINATION NODE.  */
/**************************************************************************/

void CopyExports( n1, n2 )
PNODE n1;
PNODE n2;
{
    register PEDGE e;

    for ( e = n1->exp; e != NULL; e = e->esucc )
	LinkExport( n2, CopyEdge( e, n2, NULL_NODE ) );
}


/**************************************************************************/
/* GLOBAL **************        CopyImports        ************************/
/**************************************************************************/
/* PURPOSE: COPY THE IMPORT LIST OF NODE n1 AND ATTACH IT TO NODE n2.  IF */
/*          atts IS TRUE, THEN EACH NEW IMPORT IS ATTACHED TO ITS COPY'S  */
/*          SOURCE NODE COPY'S EXPORT LIST.                               */
/**************************************************************************/

void CopyImports( n1, n2, atts )
PNODE n1;
PNODE n2;
int   atts;
{
    register PEDGE i;
    register PEDGE ii;

    for ( i = n1->imp; i != NULL; i = i->isucc ) {
        ii = CopyEdge( i, NULL_NODE, n2 );

	LinkImport( n2, ii );

	if ( atts && (!IsConst(i)) )
	    LinkExport( i->src->copy, ii );
	}
}


/**************************************************************************/
/* GLOBAL **************          CopyNode         ************************/
/**************************************************************************/
/* PURPOSE: RETURN A COPY OF NODE n.  IF n IS A GRAPH NODE, THEN EACH     */
/*          NODE IN ITS NODE LIST IS COPIED ALONG WITH ITS IMPORT LIST.   */
/*          IF n IS A COMPOUND NODE, THEN EACH SUBGRAPH IN ITS SUBGRAPH   */
/*          LIST IS COPIED.                                               */
/**************************************************************************/

PNODE CopyNode( n )
PNODE n;
{
    register PNODE nn;
    register PNODE nd;
    register PNODE p;
    register PNODE g;

    n->copy = nn = NodeAlloc( n->label, n->type );

    CopyPragmas( n, nn );

    nn->info    = n->info;
    nn->gname   = n->gname;
    nn->alst    = n->alst;
    nn->scnt    = n->scnt;
    nn->if1line = n->if1line;

    switch ( n->type ) {
	case IFIGraph:
	    break;

        case IFSGraph:
	    nn->G_DAD = n->G_DAD->copy;
        case IFLGraph:
	case IFXGraph:
	    p = nn;

	    for ( nd = n->G_NODES; nd != NULL; nd = nd->nsucc ) {
		LinkNode( p, CopyNode( nd ) );
		p = nd->copy;
		}

            CopyImports( n, nn, TRUE );

	    for ( nd = n->G_NODES; nd != NULL; nd = nd->nsucc )
		CopyImports( nd, nd->copy, TRUE );

            break;

	case IFSelect:
	case IFTagCase:
	case IFForall:
	case IFLoopA:
	case IFLoopB:
	    p = nn;

	    for ( g = n->C_SUBS; g != NULL; g = g->gsucc ) {
		LinkGraph( p, CopyNode( g ) );
		p = g->copy;
		}

            break;

	default:
	    break;
        }

    return( nn );
}
