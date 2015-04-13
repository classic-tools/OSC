#define ARRAY     struct Array
#define ARRAYP    struct Array *
#define PHYS      struct ArrayPhys
#define PHYSP     struct ArrayPhys *
#define BUFFER    struct MemoryBuffer
#define BUFFERP   struct MemoryBuffer *
#define PSBUFFER  struct PointerSwapBuffer
#define PSBUFFERP struct PointerSwapBuffer *

#define ABld(x,y,z) \
{ \
  register PHYSP Phys; \
  Phys = (PHYSP) Alloc( sizeof(PHYS) ); \
  Phys->Size = 0; \
  MY_INIT_LOCK( &Phys->Mutex ); \
  Phys->RefCount = 1; \
  Phys->ExpHistory = 1; \
  Phys->Free = 0; \
  x = (POINTER) Alloc( sizeof(ARRAY) ); \
  ((ARRAYP)x)->Size = 0; \
  ((ARRAYP)x)->LoBound = y; \
  ((ARRAYP)x)->Phys = Phys; \
  ((ARRAYP)x)->RefCount = z; \
  MY_INIT_LOCK( &((ARRAYP)x)->Mutex ); \
}

#define OptABld(x,y,z,w,u) \
{ \
  register PHYSP Phys; \
  register int  Size; \
  Size = (w)-(y)+1; \
  Size = (Size < 0)? 0 : Size; \
  x = (POINTER) Alloc(sizeof(ARRAY)); \
  MY_INIT_LOCK( &(((ARRAYP)x)->Mutex) ); \
  ((ARRAYP)x)->RefCount = z; \
  ((ARRAYP)x)->Phys = Phys = (PHYSP) Alloc(SIZE_OF(PHYS)+(sizeof(u)*Size)); \
  MY_INIT_LOCK( &Phys->Mutex ); \
  Phys->Size = Size; \
  Phys->RefCount = 1; \
  Phys->Free = Size; \
  Phys->Base = ((ARRAYP)x)->Base = (POINTER) ALIGNED_INC(PHYS,Phys); \
  ((ARRAYP)x)->Size = 0; \
  ((ARRAYP)x)->LoBound = y; \
  ((ARRAYP)x)->Phys = Phys; \
  ((ARRAYP)x)->Base = (POINTER) (((u*)(Phys->Base))-(y)); \
}


/*
** AGather( x = target array, y = value, z = component type ) XX
*/
#define AGather(x,y,z) \
{ \
  register PHYSP Phys; \
  Phys = ((ARRAYP)x)->Phys; \
  if ( Phys->Free == 0 ) {\
    IncDataCopies( sizeof(z) * ((ARRAYP)x)->Size ); \
    Phys = DoPhysExpand( x, sizeof( z ) ); \
    } \
  Phys->Free--; \
  Phys->Size++; \
  *((z*)(((ARRAYP)x)->Base)+(((ARRAYP)x)->LoBound + ((ARRAYP)x)->Size)) = y; \
  ((ARRAYP)x)->Size++; \
}

struct ArrayPhys {
  int       Size;               /* ABSOLUTE SIZE OF PHYSICAL SPACE    */ 
  POINTER   Base;               /* FIRST CELL OF PHYSICAL SPACE       */
  LOCK_TYPE Mutex;              /* LOCK FOR PHYSICAL BLOCK            */
  int       RefCount;           /* CURRENT NUMBER OF REFERENCES       */
  int       ExpHistory;         /* COUNT OF PREVIOUS EXPANSIONS       */
  int       Free;               /* UNOCCUPIED CELLS TO THE RIGHT      */
  POINTER   Dope;               /* UTILITY FIELD                      */
  };

struct Array {
  POINTER   Base;               /* RELATIVE BASE                      */
  int       LoBound;            /* LOWER BOUND                        */
  int       Size;               /* LOGICAL ARRAY SIZE                 */
  PHYSP     Phys;               /* POINTER TO PHYSICAL BLOCK          */
  LOCK_TYPE Mutex;              /* LOCK FOR DOPE VECTOR               */
  int       RefCount;           /* CURRENT NUMBER OF REFERENCES       */
  int       Mutable;            /* INTERFACE MUTABILITY               */
  };

struct MemoryBuffer {
  PHYSP    Phys;
  POINTER  Base;
  };

struct PointerSwapBuffer {
  POINTER Current;             /* THE STORAGE TO USE ON THIS ITERATION     */
  POINTER Next;                /* THE STORAGE TO USE ON THE NEXT ITERATION */
  POINTER Temp;                /* POINTER SWAP TEMPORARY VARIABLE          */
  int     InfoTop;             /* Info TOP POINTER                         */
  int     Info[6];             /* Info ARRAY FOR REGION                    */
  };

extern POINTER  ArrayDuplicatePlus();
extern POINTER  ArrayDuplicate();
extern PHYSP    DoPhysExpand();

extern POINTER	ReadBoolVector();
extern void	WriteBoolVector();

extern POINTER	ReadCharVector();
extern void	WriteCharVector();

extern POINTER	ReadDoubleVector();
extern void	WriteDoubleVector();

extern POINTER	ReadIntegerVector();
extern void	WriteIntegerVector();

extern POINTER	ReadNullVector();
extern void	WriteNullVector();

extern POINTER	ReadRealVector();
extern void	WriteRealVector();
