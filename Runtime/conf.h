#define POINTER   char*

#define True      1
#define False     0

#ifdef __STDC__
/* Use ANSI limits (if defined) to fix MaxInt problem on Crays -- 921004 PJM */

# ifndef INT_MAX
#   include <limits.h>
# endif
# define MaxInt          INT_MAX

# ifndef INT_MIN
#   include <limits.h>
# endif
# define MinInt          INT_MIN

#else
# define MaxInt          ((int)(((unsigned int)-1) >> 1))
# define MinInt          -MaxInt
#endif

#ifdef CRAY
#define MaxFloat         2.7e2465
#define MinFloat        -2.7e2465
#define MaxDouble        2.7e2465
#define MinDouble       -2.7e2465
#endif

#if SUNIX || RS6000
#define MaxFloat         1.7e38
#define MinFloat        -1.7e38
#define MaxDouble        1.7e38
#define MinDouble       -1.7e38
#endif

#if SYMMETRY || BALANCE || ALLIANT || ENCORE || SGI || SUN
#define MaxFloat         1.9e38
#define MinFloat        -1.9e38
#define MaxDouble       1.7e308
#define MinDouble      -1.7e308
#endif

#if SYMMETRY || BALANCE
#else
#define shared
#endif

#ifndef SGI
#define volatile
#endif

/* POINTER to CHARACTER CAST TO DO POINTER to CHARACTER MATHEMATICS */
#ifdef CRAY
#define PCMCAST char*
#else
#define PCMCAST int
#endif

/* ------------------------------------------------------------ */
/* Alignment size						*/
/* ------------------------------------------------------------ */

#ifdef ALLIANT
#define ALIGN_SIZE SIZEOF(double)
#endif

#if CRAY
#define ALIGN_SIZE SIZEOF(int)
#endif

#ifdef SGI
#define ALIGN_SIZE SIZEOF(int)
#endif

#ifdef HPUXPA
#define ALIGN_SIZE SIZEOF(double)
#endif

#ifndef ALIGN_SIZE
#define ALIGN_SIZE SIZEOF(double)
#endif

/* ------------------------------------------------------------ */

#define ALIGN(x,y)       ((x)((((int)y)+(ALIGN_SIZE))&(~((ALIGN_SIZE)-1))))
#define SIZE_OF(x)       (((SIZEOF(x)/(ALIGN_SIZE))+1)*(ALIGN_SIZE))
#define ALIGNED_INC(x,y) (((PCMCAST)y)+SIZE_OF(x))

#ifdef CRAY
#define float double
#endif
