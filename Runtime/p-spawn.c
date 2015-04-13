#include "world.h"

/* ------------------------------------------------------------ */
#ifdef ALLIANT
int InParallel = FALSE;

static int Transfer( ProcId, ARs )
int ProcId;
struct ActRec *ARs;
{
  register struct ActRec *MyAR;

  MyAR = &ARs[ProcId];

  (*(MyAR->ChildCode))( MyAR->ArgPointer, 
			MyAR->SliceBounds[1], MyAR->SliceBounds[2] );
}
#endif


/* ------------------------------------------------------------ */
LOCK_TYPE *UtilityLock;
LOCK_TYPE *SUtilityLock;

void InitSpawn()
{
  UtilityLock = (LOCK_TYPE*) SharedMalloc( SIZEOF(LOCK_TYPE) );
  MY_INIT_LOCK( UtilityLock );

  SUtilityLock = (LOCK_TYPE*) SharedMalloc( SIZEOF(LOCK_TYPE) );
  MY_SINIT_LOCK( SUtilityLock );
}

#define ComputeWidth(Thickness,Total,Long,Short,ARList)\
{\
  /* ------------------------------------------------------------ */\
  /* Assumes we have High, Low, MinSlice, SliceWidth, & LoopSliceCount */\
  /* defined outside the scope of the macro.			  */\
  /* ------------------------------------------------------------ */\
  Thickness     = High - Low + 1;\
  SliceWidth	= Thickness / LoopSliceCount;\
\
  /* ------------------------------------------------------------ */\
  /* Adjust so that minimum width is satisfied			  */\
  /* ------------------------------------------------------------ */\
  if ( SliceWidth >= MinSlice ) {\
    Long          = Thickness % LoopSliceCount;\
    Total	  = LoopSliceCount;\
    ARList	  = (struct ActRec *) Alloc(SIZEOF(struct ActRec)*Total );\
  } else if ( Thickness <= 0 ) {\
    Long	  = 0;\
    Total	  = 0;\
    ARList	  = NULL;\
  } else if ( Thickness < MinSlice ) {\
    SliceWidth	  = Thickness;\
    Long	  = 0;\
    Total	  = 1;\
    ARList	  = (struct ActRec *) Alloc( SIZEOF(struct ActRec) );\
  } else {\
    int		Remainder;\
    Total	  = Thickness / MinSlice; /* How many can we run? */\
    Remainder	  = Thickness % MinSlice; /* How many odd ones? */\
    SliceWidth	  = MinSlice + (Remainder/Total); /* Spread the work */\
    Long	  = Thickness % SliceWidth; /* Real left overs */\
    ARList	  = (struct ActRec *) Alloc( SIZEOF(struct ActRec)*Total);\
  }\
\
  Short		= Total - Long;\
}

#define Body(Step,Hi,NewLow) {				\
    SliceBounds		= NewAR->SliceBounds;		\
    SliceBounds[0]	= Step;		 /* Step */	\
    SliceBounds[1]	= Low;		 /* Start */	\
    SliceBounds[2]	= Hi;		 /* Stop */	\
    Low			= NewLow;			\
    NewAR->ArgPointer	= ArgPointer;			\
    NewAR->ChildCode	= ChildCode;			\
    NewAR->Done		= FALSE;			\
    NewAR->NextAR	= NewAR + 1;			\
    NewAR++;						\
  }

/**************************************************************************/
/* GLOBAL **************     BuildStridedSlices    ************************/
/**************************************************************************/
/* PURPOSE:  Specialized Loop Slicing					  */
/**************************************************************************/
void BuildStridedSlices(Head,Count,ChildCode,ArgPointer,Low,High,
			MinSlice,LoopSliceCount)
struct ActRec	**Head;
int		*Count;
void		(*ChildCode)();
POINTER		ArgPointer;
register int   	Low;
int		High;
int		MinSlice;
int		LoopSliceCount;
{
  register struct ActRec *NewAR;
  register int            Thickness;
  register int            SliceWidth;
  register int		  Long;
  register int		  Short;
  register int           *SliceBounds;
  register int		 Total;
  register int		 SaveLow;
  register int		 SaveLong;

  ComputeWidth(Thickness,Total,Long,Short,NewAR);

  *Count	= Total;
  *Head		= NewAR;

  SaveLow  = Low;
  SaveLong = Long;

  /* ------------------------------------------------------------ */
  /* There are zero or more bodies with extra iterations	  */
  /* ------------------------------------------------------------ */
  for(; Long; Long--) {
    Body(Total,SaveLow+Thickness-Long,Low+1);
  }

  /* ------------------------------------------------------------ */
  /* There's probably one or more bodies without extras		  */
  /* ------------------------------------------------------------ */
  for(; Short; Short--) {
    Body(Total,SaveLow+Thickness-SaveLong-Short,Low+1);
  }
}

/**************************************************************************/
/* GLOBAL **************     BuildBlockSlices      ************************/
/**************************************************************************/
/* PURPOSE:  Build blockwise loop slices				  */
/**************************************************************************/
void BuildBlockSlices(Head, Count, ChildCode, ArgPointer, Low, High, MinSlice,
		      LoopSliceCount)
struct ActRec **Head;
int            *Count;
void            (*ChildCode)();
POINTER         ArgPointer;
register int    Low;
register int    High;
int		MinSlice;
{
  register struct ActRec *NewAR;
  register int            Thickness;
  register int            SliceWidth;
  register int		  Long;
  register int		  Short;
  register int           *SliceBounds;
  register int		 Total;

  ComputeWidth(Thickness,Total,Long,Short,NewAR);

  *Count	= Total;
  *Head		= NewAR;

  /* ------------------------------------------------------------ */
  /* There are zero or more bodies with extra iterations	  */
  /* ------------------------------------------------------------ */
  for(; Long; Long--) {
    Body(1,Low+SliceWidth,Low+SliceWidth+1);
  }

  /* ------------------------------------------------------------ */
  /* There's probably one or more bodies without extras		  */
  /* ------------------------------------------------------------ */
  for(; Short; Short--) {
    Body(1,Low+SliceWidth-1,Low+SliceWidth);
  }
}
/**************************************************************************/
/* GLOBAL **************   BuildTriangleSlices   ************************/
/**************************************************************************/
/* PURPOSE:  Build triangular blockwise loop slices			  */
/**************************************************************************/
void BuildTriangleSlices(Head, Count, ChildCode, ArgPointer, Low, TrueHigh,
		      MinSlice, LoopSliceCount)
struct ActRec **Head;
int            *Count;
void            (*ChildCode)();
POINTER         ArgPointer;
register int    Low;
register int    TrueHigh;
int		MinSlice;
{
  register struct ActRec *NewAR;
  register int            Thickness;
  register int		  FullThick;
  register int		  High;
  register int            SliceWidth;
  register int		  Long;
  register int		  Short;
  register int           *SliceBounds;
  register int		 Total;

  FullThick = TrueHigh-Low+1;

  if ( FullThick % 2 == 1 ) {
    SisalError("Odd thick in triangle","no good for now");
  } else {
    High    = Low + ( FullThick / 2 ) -1;

    ComputeWidth(Thickness,Total,Long,Short,NewAR);
    
  }

  *Count	= Total;
  *Head		= NewAR;

  /* ------------------------------------------------------------ */
  /* There are zero or more bodies with extra iterations	  */
  /* ------------------------------------------------------------ */
  for(; Long; Long--) {
    Body(TrueHigh,Low+SliceWidth,Low+SliceWidth+1);
  }

  /* ------------------------------------------------------------ */
  /* There's probably one or more bodies without extras		  */
  /* ------------------------------------------------------------ */
  for(; Short; Short--) {
    Body(TrueHigh,Low+SliceWidth-1,Low+SliceWidth);
  }
}

/**************************************************************************/
/* GLOBAL **************      BuildGssSlices       ************************/
/**************************************************************************/
/* PURPOSE:  Build slices according to the guided self scheduling algor.  */
/**************************************************************************/
void BuildGssSlices( Head, Count, ChildCode, ArgPointer, Low, High,
		    MinSlice, LoopSliceCount )
struct ActRec **Head;
int	      *Count;
void	      (*ChildCode)();
POINTER	      ArgPointer;
int	      Low;
int	      High;
{
  register struct ActRec *NewAR;
  register int            ThisHi;
  register int           *SliceBounds;
  register int            LowTemp;

  *Count = 0;
  LowTemp = Low;

  while ( LowTemp <= High ) {
    LowTemp += ((High-LowTemp)/NumWorkers) + 1;
    *Count += 1;
  }

  *Head = NewAR = (struct ActRec *) Alloc(SIZEOF(struct ActRec)*(*Count) );

  while ( Low <= High ) {
    SliceBounds    = NewAR->SliceBounds;
    SliceBounds[0] = 1;		/* Single step through iterations */
    SliceBounds[1] = Low;
    SliceBounds[2] = ThisHi = Low + ((High-Low)/NumWorkers);

    NewAR->ArgPointer  = ArgPointer;
    NewAR->ChildCode   = ChildCode;
    NewAR->Done        = FALSE;

    Low = ThisHi + 1;

    NewAR->NextAR = NewAR + 1;
    NewAR++;
  }
}

/**************************************************************************/
/* GLOBAL **************        BuildSlices        ************************/
/**************************************************************************/
/* PURPOSE:  Choose an appropriate style and generate loop slices AR's	  */
/*	     accordingly.  This routine is called by the generic BSlice	  */
/*	     macro.							  */
/**************************************************************************/
void BuildSlices( LoopType, Head, Count, ChildCode, ArgPointer, Low, High,
		 MinSlice, LoopSliceCount )
int             LoopType;
struct ActRec **Head;
int            *Count;
void            (*ChildCode)();
POINTER         ArgPointer;
int             Low;
int             High;
int		MinSlice;
int		LoopSliceCount;
{
  switch ( DefaultLoopStyle ) {
   case 'G': /* GSS */
    if ( LoopType == SPAWN_COMPLEX ) {
      BuildGssSlices( Head, Count, ChildCode, ArgPointer, Low, High,
		     MinSlice, LoopSliceCount );
      break;
    }
    /* else fall through and use block slicing */

   case 'B': /* BLOCK */
    BuildBlockSlices( Head, Count, ChildCode, ArgPointer, Low, High,
		     MinSlice, LoopSliceCount );
    break;

   case 'S': /* STRIDED */
    BuildStridedSlices( Head, Count, ChildCode, ArgPointer, Low, High,
		       MinSlice, LoopSliceCount );
    break;

   default:
    SisalError("BuildSlices","Invalid default loop style");
  }
}

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
void OptSpawnSlices( FirstAR, Count )
struct  ActRec *FirstAR;
int     Count;
{
  register struct ActRec *LastAR;

  if ( Count > 0 ) {

    if ( Count == 1 ) {
      (*(FirstAR->ChildCode))( FirstAR->ArgPointer, 
			       FirstAR->SliceBounds[1], 
			       FirstAR->SliceBounds[2],
			       FirstAR->SliceBounds[0]
			      );
      return;
      }

#ifdef ALLIANT
    if ( InParallel ) {
      register int Id;
      for ( Id = 0; Id < Count; Id++ )
	Transfer( Id, FirstAR );
    } else {
      InParallel = TRUE;
      concurrent_call( 0, Transfer, Count, FirstAR );
      InParallel = FALSE;
      }
    goto TheExit;
#endif

    LastAR = FirstAR +(Count-1);

    RListEnQ( FirstAR, LastAR );
    Sync( FirstAR, LastAR+1 );
    }

#ifdef ALLIANT
TheExit:
#endif
  return;
}

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
/* Specialized Spawns */
/* ------------------------------------------------------------ */
void SpawnStridedSlices( ChildCode, ArgPointer, Low, High, MinSlice, LoopSliceCount )
void    (*ChildCode)();
POINTER ArgPointer;
int     Low;
int     High;
int	MinSlice;
int	LoopSliceCount;
{
  struct ActRec *FirstAR;
  int            Count;

  BuildStridedSlices(&FirstAR,&Count,ChildCode,ArgPointer,Low,High,MinSlice,LoopSliceCount);

  OptSpawnSlices(FirstAR,Count);

  DeAlloc((POINTER)FirstAR);
}
/* ------------------------------------------------------------ */
void SpawnBlockSlices( ChildCode, ArgPointer, Low, High, LoopSliceCount )
void    (*ChildCode)();
POINTER ArgPointer;
int     Low;
int     High;
int	LoopSliceCount;
{
  struct ActRec *FirstAR;
  int            Count;

  BuildBlockSlices(&FirstAR,&Count,ChildCode,ArgPointer,Low,High,1,LoopSliceCount);

  OptSpawnSlices(FirstAR,Count);

  DeAlloc((POINTER)FirstAR);
}

/* ------------------------------------------------------------ */
void SpawnGssSlices( ChildCode, ArgPointer, Low, High, MinSlice, LoopSliceCount)
void    (*ChildCode)();
POINTER ArgPointer;
int     Low;
int     High;
int	MinSlice;
int	LoopSliceCount;
{
  struct ActRec *FirstAR;
  int            Count;

  BuildGssSlices(&FirstAR,&Count,ChildCode,ArgPointer,Low,High,MinSlice,LoopSliceCount);

  OptSpawnSlices(FirstAR,Count);

  DeAlloc((POINTER)FirstAR);
}

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
void SpawnSlices( LoopType, ChildCode, ArgPointer, Low, High, MinSlice, LoopSliceCount )
int     LoopType;         /* CONCUR_VECTOR or CONCUR */
void    (*ChildCode)();
POINTER ArgPointer;
int     Low;
int     High;
int	MinSlice;
int	LoopSliceCount;
{
  struct ActRec *FirstAR;
  int            Count;

  BuildSlices( LoopType, &FirstAR, &Count, ChildCode, ArgPointer, Low, High, MinSlice, LoopSliceCount );

  OptSpawnSlices( FirstAR, Count );

  DeAlloc( (POINTER)FirstAR );
}

