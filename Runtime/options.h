if (argv[idx][0] == '-' ) {                                              /* M */
  CorrectUsage = NULL;                                                   /* a */
  switch( argv[idx][1]) {                                                /* c */
                                                                         /* h */
   case 'X':                                                             /* i */
    if ( strcmp(argv[idx]+1,"X") == 0 ) {                                /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                              -X                              */     /* G */
  /*       All remaining arguments treated as FIBRE inputs        */     /* e */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
            idx++;                  /* Skip to next argument */          /* r */
            while ( idx < argc ) {                                       /* a */
              (void)strcat(ArgumentString,argv[idx]);                    /* t */
              (void)strcat(ArgumentString," ");                          /* e */
              idx++;                                                     /* d */
            }                                                            /*   */
                                                                         /* D */
      break;                                                             /* o */
    }                                                                    /*   */
    goto OptionError;                                                    /* N */
                                                                         /* o */
   case 'a':                                                             /* t */
    if ( argv[idx][2] == 'x' && isdigit(argv[idx][3]) ) {                /*   */
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                           -ax<num>                           */     /* a */
  /*                  Set array expansion factor                  */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      if ( GET_Tmp( 3 ) < 0 ) goto OptionError;                          /*   */
      ArrayExpansion = Tmp;                                              /* M */
                                                                         /* a */
      break;                                                             /* c */
    }                                                                    /* h */
    goto OptionError;                                                    /* i */
                                                                         /* n */
   case 'b':                                                             /* e */
    if ( strcmp(argv[idx]+1,"b") == 0 ) {                                /*   */
                                                                         /* G */
  /* ------------------------------------------------------------ */     /* e */
  /*                              -b                              */     /* n */
  /*  Use distributed run queue system to bind work to processes  */     /* e */
  /* ------------------------------------------------------------ */     /* r */
                                                                         /* a */
      BindParallelWork = TRUE;                                           /* t */
                                                                         /* e */
      break;                                                             /* d */
    } else if ( strcmp(argv[idx]+1,"bformat") == 0 ) {                   /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                   -bformat <formatstring>                    */     /*   */
  /*           Change the default boolean output format           */     /* N */
  /* ------------------------------------------------------------ */     /* o */
    CorrectUsage = "Usage: -bformat <formatstring>";                     /* t */
    if ( (++idx) >= argc ) goto OptionError;                             /*   */
                                                                         /* C */
      bformat = ParseCEscapes(argv[idx]);                                /* h */
                                                                         /* a */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"blocked") == 0 ) {                   /* g */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -blocked                           */     /* M */
  /*    Use a blocked scheduling algorithm for loops (default)    */     /* a */
  /* ------------------------------------------------------------ */     /* c */
                                                                         /* h */
      DefaultLoopStyle = 'B';                                            /* i */
                                                                         /* n */
      break;                                                             /* e */
    }                                                                    /*   */
    goto OptionError;                                                    /* G */
                                                                         /* e */
   case 'c':                                                             /* n */
    if ( strcmp(argv[idx]+1,"cformat") == 0 ) {                          /* e */
                                                                         /* r */
  /* ------------------------------------------------------------ */     /* a */
  /*                   -cformat <formatstring>                    */     /* t */
  /* Change the default integer output format for printable characters */
  /* ------------------------------------------------------------ */     /* e */
    CorrectUsage = "Usage: -cformat <formatstring>";                     /* d */
    if ( (++idx) >= argc ) goto OptionError;                             /*   */
                                                                         /* D */
      cformat = ParseCEscapes(argv[idx]);                                /* o */
                                                                         /*   */
      break;                                                             /* N */
    } else if ( strcmp(argv[idx]+1,"cformat2") == 0 ) {                  /* o */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /*   */
  /*                   -cformat2 <formatstring>                   */     /* C */
  /*    Change the default uprintable character output format     */     /* h */
  /* ------------------------------------------------------------ */     /* a */
    CorrectUsage = "Usage: -cformat2 <formatstring>";                    /* n */
    if ( (++idx) >= argc ) goto OptionError;                             /* g */
                                                                         /* e */
      cformat2 = ParseCEscapes(argv[idx]);                               /*   */
                                                                         /* M */
      break;                                                             /* a */
    }                                                                    /* c */
    goto OptionError;                                                    /* h */
                                                                         /* i */
   case 'd':                                                             /* n */
    if ( strcmp(argv[idx]+1,"dformat") == 0 ) {                          /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* G */
  /*                   -dformat <formatstring>                    */     /* e */
  /*         Change the default double real output format         */     /* n */
  /* ------------------------------------------------------------ */     /* e */
    CorrectUsage = "Usage: -dformat <formatstring>";                     /* r */
    if ( (++idx) >= argc ) goto OptionError;                             /* a */
                                                                         /* t */
      dformat = ParseCEscapes(argv[idx]);                                /* e */
                                                                         /* d */
      break;                                                             /*   */
    } else if ( argv[idx][2] == 's' && isdigit(argv[idx][3]) ) {         /* D */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -ds<num>                           */     /* N */
  /*  Initialize the shared data management pool to <num> bytes.  */     /* o */
  /* ------------------------------------------------------------ */     /* t */
                                                                         /*   */
      if ( GET_Tmp( 3 ) <= 0 ) goto OptionError;                         /* C */
      DsaSize = Tmp;                                                     /* h */
                                                                         /* a */
      break;                                                             /* n */
    } else if ( argv[idx][2] == 'x' && isdigit(argv[idx][3]) ) {         /* g */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -dx<num>                           */     /* M */
  /* Set the exact fit storage allocation threshold to <num> bytes. */   /* a */
  /* ------------------------------------------------------------ */     /* c */
                                                                         /* h */
      if ( GET_Tmp( 3 ) < 0 ) goto OptionError;                          /* i */
                                                                         /* n */
      XftThreshold = Tmp;                                                /* e */
                                                                         /*   */
      break;                                                             /* G */
    }                                                                    /* e */
    goto OptionError;                                                    /* n */
                                                                         /* e */
   case 'f':                                                             /* r */
    if ( strcmp(argv[idx]+1,"fformat") == 0 ) {                          /* a */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /* e */
  /*                   -fformat <formatstring>                    */     /* d */
  /*            Change the default real  output format            */     /*   */
  /* ------------------------------------------------------------ */     /* D */
    CorrectUsage = "Usage: -fformat <formatstring>";                     /* o */
    if ( (++idx) >= argc ) goto OptionError;                             /*   */
                                                                         /* N */
      fformat = ParseCEscapes(argv[idx]);                                /* o */
                                                                         /* t */
      break;                                                             /*   */
    }                                                                    /* C */
    goto OptionError;                                                    /* h */
                                                                         /* a */
   case 'g':                                                             /* n */
    if ( strcmp(argv[idx]+1,"gss") == 0 ) {                              /* g */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                             -gss                             */     /* M */
  /* Use guided self scheduling algorithm instead of block slicing */    /* a */
  /* ------------------------------------------------------------ */     /* c */
                                                                         /* h */
      DefaultLoopStyle = 'G';                                            /* i */
                                                                         /* n */
      break;                                                             /* e */
    }                                                                    /*   */
    goto OptionError;                                                    /* G */
                                                                         /* e */
   case 'h':                                                             /* n */
    if ( strcmp(argv[idx]+1,"help") == 0 ) {                             /* e */
                                                                         /* r */
  /* ------------------------------------------------------------ */     /* a */
  /*                            -help                             */     /* t */
  /*                        Same as -usage                        */     /* e */
  /* ------------------------------------------------------------ */     /* d */
                                                                         /*   */
      PrintUsageTable(argv,idx);                                         /* D */
                                                                         /* o */
      break;                                                             /*   */
    }                                                                    /* N */
    goto OptionError;                                                    /* o */
                                                                         /* t */
   case 'i':                                                             /*   */
    if ( strcmp(argv[idx]+1,"iformat") == 0 ) {                          /* C */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* a */
  /*                   -iformat <formatstring>                    */     /* n */
  /*           Change the default integer output format           */     /* g */
  /* ------------------------------------------------------------ */     /* e */
    CorrectUsage = "Usage: -iformat <formatstring>";                     /*   */
    if ( (++idx) >= argc ) goto OptionError;                             /* M */
                                                                         /* a */
      iformat = ParseCEscapes(argv[idx]);                                /* c */
                                                                         /* h */
      break;                                                             /* i */
    }                                                                    /* n */
    goto OptionError;                                                    /* e */
                                                                         /*   */
   case 'l':                                                             /* G */
    if ( argv[idx][2] == 's' && isdigit(argv[idx][3]) ) {                /* e */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                           -ls<num>                           */     /* r */
  /*             Set the default slice count to <num>             */     /* a */
  /* ------------------------------------------------------------ */     /* t */
                                                                         /* e */
      if ( GET_Tmp( 3 ) <= 0 ) goto OptionError;                         /* d */
                                                                         /*   */
      LoopSlices = Tmp;                                                  /* D */
                                                                         /* o */
      break;                                                             /*   */
    }                                                                    /* N */
    goto OptionError;                                                    /* o */
                                                                         /* t */
   case 'n':                                                             /*   */
    if ( strcmp(argv[idx]+1,"nb") == 0 ) {                               /* C */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* a */
  /*                             -nb                              */     /* n */
  /*                  Do not bind parallel work                   */     /* g */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /*   */
      BindParallelWork = FALSE;                                          /* M */
                                                                         /* a */
      break;                                                             /* c */
    } else if ( strcmp(argv[idx]+1,"nformat") == 0 ) {                   /* h */
                                                                         /* i */
  /* ------------------------------------------------------------ */     /* n */
  /*                   -nformat <formatstring>                    */     /* e */
  /*          Change the default null type output format          */     /*   */
  /* ------------------------------------------------------------ */     /* G */
    CorrectUsage = "Usage: -nformat <formatstring>";                     /* e */
    if ( (++idx) >= argc ) goto OptionError;                             /* n */
                                                                         /* e */
      nformat = ParseCEscapes(argv[idx]);                                /* r */
                                                                         /* a */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"nostrings") == 0 ) {                 /* e */
                                                                         /* d */
  /* ------------------------------------------------------------ */     /*   */
  /*                          -nostrings                          */     /* D */
  /*   Do not print array[character] in string (quoted) format    */     /* o */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* N */
      FibreStrings = FALSE;                                              /* o */
                                                                         /* t */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"nqs") == 0 ) {                       /* C */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* a */
  /*                             -nqs                             */     /* n */
  /*       ...options  Submit as NQS batch job (Cray only)        */     /* g */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /*   */
      SubmitNQS(argc,argv,idx);                                          /* M */
                                                                         /* a */
      break;                                                             /* c */
    }                                                                    /* h */
    goto OptionError;                                                    /* i */
                                                                         /* n */
   case 'r':                                                             /* e */
    if ( strcmp(argv[idx]+1,"r") == 0 ) {                                /*   */
                                                                         /* G */
  /* ------------------------------------------------------------ */     /* e */
  /*                              -r                              */     /* n */
  /*  Append resource utilization information to the file s.info  */     /* e */
  /* ------------------------------------------------------------ */     /* r */
                                                                         /* a */
      GatherPerfInfo = TRUE;                                             /* t */
                                                                         /* e */
      break;                                                             /* d */
    }                                                                    /*   */
    goto OptionError;                                                    /* D */
                                                                         /* o */
   case 's':                                                             /*   */
    if ( strcmp(argv[idx]+1,"strided") == 0 ) {                          /* N */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /* t */
  /*                           -strided                           */     /*   */
  /* Use strided loop scheduling algorithm instead of block slicing */   /* C */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* a */
      DefaultLoopStyle = 'S';                                            /* n */
                                                                         /* g */
      break;                                                             /* e */
    }                                                                    /*   */
    goto OptionError;                                                    /* M */
                                                                         /* a */
   case 't':                                                             /* c */
    if ( strncmp(argv[idx]+1,"tr=",3) == 0 ) {                           /* h */
                                                                         /* i */
  /* ------------------------------------------------------------ */     /* n */
  /*                       -tr=<attribute>                        */     /* e */
  /*                 Interface to tracer function                 */     /*   */
  /* ------------------------------------------------------------ */     /* G */
    CorrectUsage = "Usage: -tr=<attribute>";                             /* e */
    if ( argv[idx][3] == NULL ) goto OptionError;                        /* n */
                                                                         /* e */
       ParseTracerCommand(argv[idx]+4);                                  /* r */
                                                                         /* a */
      break;                                                             /* t */
    }                                                                    /* e */
    goto OptionError;                                                    /* d */
                                                                         /*   */
   case 'u':                                                             /* D */
    if ( strcmp(argv[idx]+1,"usage") == 0 ) {                            /* o */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* N */
  /*                            -usage                            */     /* o */
  /*                      Produce this list                       */     /* t */
  /* ------------------------------------------------------------ */     /*   */
    PrintUsageTable(argv,idx);                                           /* C */
      break;                                                             /* h */
    }                                                                    /* a */
    goto OptionError;                                                    /* n */
                                                                         /* g */
   case 'w':                                                             /* e */
    if ( isdigit(argv[idx][2]) ) {                                       /*   */
                                                                         /* M */
  /* ------------------------------------------------------------ */     /* a */
  /*                           -w<num>                            */     /* c */
  /*           Set number of worker processes to <num>            */     /* h */
  /* ------------------------------------------------------------ */     /* i */
                                                                         /* n */
      if ( GET_Tmp( 2 ) <= 0 ) goto OptionError;                         /* e */
                                                                         /*   */
      if ( Tmp > MAX_PROCS ) goto OptionError;                           /* G */
                                                                         /* e */
      NumWorkers = Tmp;                                                  /* n */
                                                                         /* e */
      break;                                                             /* r */
    }                                                                    /* a */
    goto OptionError;                                                    /* t */
                                                                         /* e */
   case 'z':                                                             /* d */
    if ( strcmp(argv[idx]+1,"z") == 0 ) {                                /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                              -z                              */     /*   */
  /*              Do not print the program's output.              */     /* N */
  /* ------------------------------------------------------------ */     /* o */
                                                                         /* t */
          NoFibreOutput = TRUE;                                          /*   */
                                                                         /* C */
      break;                                                             /* h */
    }                                                                    /* a */
    goto OptionError;                                                    /* n */
   default: goto OptionError;                                            /* g */
  }                                                                      /* e */
}                                                                        /*   */
