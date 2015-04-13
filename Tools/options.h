if (argv[idx][0] == '-' ) {                                              /* M */
  CorrectUsage = NULL;                                                   /* a */
  switch( argv[idx][1]) {                                                /* c */
                                                                         /* h */
   case '%':                                                             /* i */
    if ( strcmp(argv[idx]+1,"%") == 0 ) {                                /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                              -%                              */     /* G */
  /*   Insert SISAL source line comments in the generated code    */     /* e */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      ShowSource = TRUE;                                                 /* r */
                                                                         /* a */
      break;                                                             /* t */
    }                                                                    /* e */
    goto OptionError;                                                    /* d */
                                                                         /*   */
   case 'A':                                                             /* D */
    if ( isdigit(argv[idx][2]) ) {                                       /* o */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* N */
  /*                           -A<num>                            */     /* o */
  /*        Set loop parallelization ``At'' level to <num>        */     /* t */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* C */
      atlevel = argv[idx];                                               /* h */
                                                                         /* a */
      break;                                                             /* n */
    }                                                                    /* g */
    goto OptionError;                                                    /* e */
                                                                         /*   */
   case 'C':                                                             /* M */
    if ( strcmp(argv[idx]+1,"C") == 0 ) {                                /* a */
                                                                         /* c */
  /* ------------------------------------------------------------ */     /* h */
  /*                              -C                              */     /* i */
  /*                 Stop after generating C code                 */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /*   */
      stopC = TRUE;                                                      /* G */
                                                                         /* e */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"CPP") == 0 ) {                       /* e */
                                                                         /* r */
  /* ------------------------------------------------------------ */     /* a */
  /*                             -CPP                             */     /* t */
  /*   Run the SISAL files through the C preprocessor and stop    */     /* e */
  /* ------------------------------------------------------------ */     /* d */
                                                                         /*   */
      stopCPP = TRUE;                                                    /* D */
      nocpp = FALSE;                /* -CPP implies -cpp PJM 10/92 */    /* o */
                                                                         /*   */
      break;                                                             /* N */
    } else if ( strncmp(argv[idx]+1,"CC=",3) == 0 ) {                    /* o */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /*   */
  /*                          -CC=<path>                          */     /* C */
  /*                 Use <path> as the C compiler                 */     /* h */
  /* ------------------------------------------------------------ */     /* a */
    CorrectUsage = "Usage: -CC=<path>";                                  /* n */
    if ( argv[idx][3] == NULL ) goto OptionError;                        /* g */
                                                                         /* e */
      cc = &argv[idx][4];                                                /*   */
      newcc = TRUE;                                                      /* M */
                                                                         /* a */
      break;                                                             /* c */
    }                                                                    /* h */
    goto OptionError;                                                    /* i */
                                                                         /* n */
   case 'D':                                                             /* e */
    if ( strcmp(argv[idx]+1,"D") == 0 ) {                                /*   */
                                                                         /* G */
  /* ------------------------------------------------------------ */     /* e */
  /*                              -D                              */     /* n */
  /*                Use DEBUG version of frontend                 */     /* e */
  /* ------------------------------------------------------------ */     /* r */
                                                                         /* a */
      debug = TRUE;                                                      /* t */
      DeBuG = TRUE;                                                      /* e */
                                                                         /* d */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"DI") == 0 ) {                        /* D */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /*   */
  /*                             -DI                              */     /* N */
  /*                    Compile to DI stye IF1                    */     /* o */
  /* ------------------------------------------------------------ */     /* t */
                                                                         /*   */
      stopDI = TRUE;                                                     /* C */
                                                                         /* h */
      break;                                                             /* a */
    } else {                                                             /* n */
                                                                         /* g */
  /* ------------------------------------------------------------ */     /* e */
  /*                           -D<defn>                           */     /*   */
  /*               Define macro for C preprocessor                */     /* M */
  /* ------------------------------------------------------------ */     /* a */
    CorrectUsage = "Usage: -D<defn>";                                    /* c */
    if ( argv[idx][1] == NULL ) goto OptionError;                        /* h */
                                                                         /* i */
      cppoptions[++cppoptioncnt] = argv[idx];                            /* n */
                                                                         /* e */
      break;                                                             /*   */
    }                                                                    /* G */
                                                                         /* e */
   case 'F':                                                             /* n */
    if ( strncmp(argv[idx]+1,"FF=",3) == 0 ) {                           /* e */
                                                                         /* r */
  /* ------------------------------------------------------------ */     /* a */
  /*                          -FF=<path>                          */     /* t */
  /*              Use <path> as the FORTRAN compiler              */     /* e */
  /* ------------------------------------------------------------ */     /* d */
    CorrectUsage = "Usage: -FF=<path>";                                  /*   */
    if ( argv[idx][3] == NULL ) goto OptionError;                        /* D */
                                                                         /* o */
      ff = &argv[idx][4];                                                /*   */
      newff = TRUE;                                                      /* N */
                                                                         /* o */
      break;                                                             /* t */
    }                                                                    /*   */
    goto OptionError;                                                    /* C */
                                                                         /* h */
   case 'I':                                                             /* a */
    if ( strcmp(argv[idx]+1,"IF1") == 0 ) {                              /* n */
                                                                         /* g */
  /* ------------------------------------------------------------ */     /* e */
  /*                             -IF1                             */     /*   */
  /*                   Compile to IF1 and stop                    */     /* M */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* c */
      stopIF1 = TRUE;                                                    /* h */
                                                                         /* i */
      break;                                                             /* n */
    } else if ( isdigit(argv[idx][2]) ) {                                /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* G */
  /*                           -I<num>                            */     /* e */
  /*            Set assumed iteration count for loops             */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* r */
      iter = argv[idx];                                                  /* a */
      iter[1] = '@';                                                     /* t */
                                                                         /* e */
      break;                                                             /* d */
    } else {                                                             /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                           -I<path>                           */     /*   */
  /*              Search <path> for C include files               */     /* N */
  /* ------------------------------------------------------------ */     /* o */
    CorrectUsage = "Usage: -I<path>";                                    /* t */
    if ( argv[idx][1] == NULL ) goto OptionError;                        /*   */
                                                                         /* C */
      cppoptions[++cppoptioncnt] = argv[idx];                            /* h */
                                                                         /* a */
      break;                                                             /* n */
    }                                                                    /* g */
                                                                         /* e */
   case 'K':                                                             /*   */
    if ( strcmp(argv[idx]+1,"Keep") == 0 ) {                             /* M */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* c */
  /*                            -Keep                             */     /* h */
  /*       Keep intermediate files instead of deleting them       */     /* i */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      Keep = TRUE;                                                       /*   */
      TMPDIR="";                                                         /* G */
                                                                         /* e */
      break;                                                             /* n */
    }                                                                    /* e */
    goto OptionError;                                                    /* r */
                                                                         /* a */
   case 'L':                                                             /* t */
    if ( isdigit(argv[idx][2]) ) {                                       /* e */
                                                                         /* d */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -L<num>                            */     /* D */
  /*        Set nested parallelization threshold to <num>         */     /* o */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* N */
      level = argv[idx];                                                 /* o */
      level[1] = 'L';                                                    /* t */
                                                                         /*   */
      break;                                                             /* C */
    } else if ( strncmp(argv[idx]+1,"Loopstyle=",10) == 0 ) {            /* h */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* n */
  /*                    -Loopstyle=<stylechar>                    */     /* g */
  /*    Set the default loop parallelism style to <stylechar>     */     /* e */
  /* ------------------------------------------------------------ */     /*   */
    CorrectUsage = "Usage: -Loopstyle=<stylechar>";                      /* M */
    if ( argv[idx][10] == NULL ) goto OptionError;                       /* a */
                                                                         /* c */
      LoopStyle[2] = argv[idx][11];                                      /* h */
                                                                         /* i */
      break;                                                             /* n */
    }                                                                    /* e */
    goto OptionError;                                                    /*   */
                                                                         /* G */
   case 'M':                                                             /* e */
    if ( strcmp(argv[idx]+1,"MEM") == 0 ) {                              /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* r */
  /*                             -MEM                             */     /* a */
  /*              Stop after memory allocation phase              */     /* t */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* d */
      stopMEM = TRUE;                                                    /*   */
                                                                         /* D */
      break;                                                             /* o */
    } else if ( strcmp(argv[idx]+1,"MONO") == 0 ) {                      /*   */
                                                                         /* N */
  /* ------------------------------------------------------------ */     /* o */
  /*                            -MONO                             */     /* t */
  /*          Stop after compiling and linking IF1 files          */     /*   */
  /* ------------------------------------------------------------ */     /* C */
                                                                         /* h */
      stopMONO = TRUE;                                                   /* a */
                                                                         /* n */
      break;                                                             /* g */
    }                                                                    /* e */
    goto OptionError;                                                    /*   */
                                                                         /* M */
   case 'N':                                                             /* a */
    if ( strcmp(argv[idx]+1,"NoSliceThrottle") == 0 ) {                  /* c */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* i */
  /*                       -NoSliceThrottle                       */     /* n */
  /*   Turn off the slice throttle feature (MinSlice estimate)    */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* G */
      MinSliceThrottle = FALSE;                                          /* e */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( isdigit(argv[idx][2]) ) {                                /* r */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* t */
  /*                           -N<num>                            */     /* e */
  /*        Set nested parallelization threshold to <num>         */     /* d */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* D */
      level = argv[idx];                                                 /* o */
      level[1] = 'L';                                                    /*   */
                                                                         /* N */
      break;                                                             /* o */
    }                                                                    /* t */
    goto OptionError;                                                    /*   */
                                                                         /* C */
   case 'O':                                                             /* h */
    if ( strcmp(argv[idx]+1,"O") == 0 ) {                                /* a */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* g */
  /*                              -O                              */     /* e */
  /*                  Use enabled optimizations                   */     /*   */
  /* ------------------------------------------------------------ */     /* M */
                                                                         /* a */
      noimp = FALSE;                                                     /* c */
      bounds = FALSE;                                                    /* h */
                                                                         /* i */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"OPT") == 0 ) {                       /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* G */
  /*                             -OPT                             */     /* e */
  /*                 Stop after IF1 optimizations                 */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* r */
      stopOPT = TRUE;                                                    /* a */
                                                                         /* t */
      break;                                                             /* e */
    }                                                                    /* d */
    goto OptionError;                                                    /*   */
                                                                         /* D */
   case 'P':                                                             /* o */
    if ( strcmp(argv[idx]+1,"PART") == 0 ) {                             /*   */
                                                                         /* N */
  /* ------------------------------------------------------------ */     /* o */
  /*                            -PART                             */     /* t */
  /*                Stop after partitioning phase                 */     /*   */
  /* ------------------------------------------------------------ */     /* C */
                                                                         /* h */
      stopPART = TRUE;                                                   /* a */
                                                                         /* n */
      break;                                                             /* g */
    } else if ( strcmp(argv[idx]+1,"Prof") == 0 ) {                      /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* M */
  /*                            -Prof                             */     /* a */
  /*                  Profile compiler execution                  */     /* c */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* i */
      Prof = TRUE;                                                       /* n */
      verbose = TRUE;                                                    /* e */
                                                                         /*   */
      break;                                                             /* G */
    } else if ( isdigit(argv[idx][2]) ) {                                /* e */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                           -P<num>                            */     /* r */
  /*    Assume <num> processors are available for partitioning    */     /* a */
  /* ------------------------------------------------------------ */     /* t */
                                                                         /* e */
      procs = argv[idx];                                                 /* d */
      procs[1] = 'P';                                                    /*   */
                                                                         /* D */
      break;                                                             /* o */
    }                                                                    /*   */
    goto OptionError;                                                    /* N */
                                                                         /* o */
   case 'S':                                                             /* t */
    if ( strcmp(argv[idx]+1,"S") == 0 ) {                                /*   */
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                              -S                              */     /* a */
  /*           Stop after generating assembly language            */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      stopS = TRUE;                                                      /*   */
      TMPDIR="";                                                         /* M */
                                                                         /* a */
      break;                                                             /* c */
    }                                                                    /* h */
    goto OptionError;                                                    /* i */
                                                                         /* n */
   case 'U':                                                             /* e */
    if ( strcmp(argv[idx]+1,"UP") == 0 ) {                               /*   */
                                                                         /* G */
  /* ------------------------------------------------------------ */     /* e */
  /*                             -UP                              */     /* n */
  /*             Stop after update in place analysis              */     /* e */
  /* ------------------------------------------------------------ */     /* r */
                                                                         /* a */
      stopUP = TRUE;                                                     /* t */
                                                                         /* e */
      break;                                                             /* d */
    } else {                                                             /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                           -U<defn>                           */     /*   */
  /*            Undefine definition for C preprocessor            */     /* N */
  /* ------------------------------------------------------------ */     /* o */
    CorrectUsage = "Usage: -U<defn>";                                    /* t */
    if ( argv[idx][1] == NULL ) goto OptionError;                        /*   */
                                                                         /* C */
      cppoptions[++cppoptioncnt] = argv[idx];                            /* h */
                                                                         /* a */
      break;                                                             /* n */
    }                                                                    /* g */
                                                                         /* e */
   case 'a':                                                             /*   */
    if ( strcmp(argv[idx]+1,"aggvector") == 0 ) {                        /* M */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* c */
  /*                          -aggvector                          */     /* h */
  /*          Aggressively fuse independent vector loops          */     /* i */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      AggressiveVector =TRUE;                                            /*   */
                                                                         /* G */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"alliantfx") == 0 ) {                 /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* r */
  /*                          -alliantfx                          */     /* a */
  /*               Compile for Alliant architecture               */     /* t */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* d */
      novec = FALSE;                                                     /*   */
      avector = TRUE;                                                    /* D */
      alliantfx = TRUE;                                                  /* o */
                                                                         /*   */
      break;                                                             /* N */
    } else if ( strcmp(argv[idx]+1,"avector") == 0 ) {                   /* o */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -avector                           */     /* C */
  /*                  Use Alliant style vectors                   */     /* h */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* n */
      novec = FALSE;                                                     /* g */
      avector =TRUE;                                                     /* e */
                                                                         /*   */
      break;                                                             /* M */
    }                                                                    /* a */
    goto OptionError;                                                    /* c */
                                                                         /* h */
   case 'b':                                                             /* i */
    if ( strcmp(argv[idx]+1,"bind") == 0 ) {                             /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                            -bind                             */     /* G */
  /*   Array descriptor data will not change in calls to SISAL    */     /* e */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      BindProcessors = TRUE;                                             /* r */
                                                                         /* a */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"bounds") == 0 ) {                    /* e */
                                                                         /* d */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -bounds                            */     /* D */
  /*          Generate code to check for various errors           */     /* o */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* N */
      bounds = TRUE;                                                     /* o */
                                                                         /* t */
      break;                                                             /*   */
    }                                                                    /* C */
    goto OptionError;                                                    /* h */
                                                                         /* a */
   case 'c':                                                             /* n */
    if ( strcmp(argv[idx]+1,"c") == 0 ) {                                /* g */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                          -c <funct>                          */     /* M */
  /*          Consider <funct> available as a C external          */     /* a */
  /* ------------------------------------------------------------ */     /* c */
    CorrectUsage = "Usage: -c <funct>";                                  /* h */
    if ( (++idx) >= argc ) goto OptionError;                             /* i */
                                                                         /* n */
      coptions[++coptioncnt] = argv[idx];                                /* e */
                                                                         /*   */
      break;                                                             /* G */
    } else if ( strcmp(argv[idx]+1,"call") == 0 ) {                      /* e */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                        -call <funct>                         */     /* r */
  /*               Call <funct> instead of inlining               */     /* a */
  /* ------------------------------------------------------------ */     /* t */
    CorrectUsage = "Usage: -call <funct>";                               /* e */
    if ( (++idx) >= argc ) goto OptionError;                             /* d */
                                                                         /*   */
      calloptions[++calloptioncnt] = argv[idx];                          /* D */
                                                                         /* o */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"cinfo") == 0 ) {                     /* N */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /* t */
  /*                            -cinfo                            */     /*   */
  /*               Get concurentization information               */     /* C */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* a */
      cinfo |= ParseMap("1");                                            /* n */
                                                                         /* g */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"concur") == 0 ) {                    /*   */
                                                                         /* M */
  /* ------------------------------------------------------------ */     /* a */
  /*                           -concur                            */     /* c */
  /*                   Generate concurrent code                   */     /* h */
  /* ------------------------------------------------------------ */     /* i */
                                                                         /* n */
      concur = TRUE;                                                     /* e */
                                                                         /*   */
      break;                                                             /* G */
    } else if ( strcmp(argv[idx]+1,"copyinfo") == 0 ) {                  /* e */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                          -copyinfo                           */     /* r */
  /*            Gather inforation on aggregate copying            */     /* a */
  /* ------------------------------------------------------------ */     /* t */
                                                                         /* e */
      cpyinfo = TRUE;                                                    /* d */
                                                                         /*   */
      break;                                                             /* D */
    } else if ( strcmp(argv[idx]+1,"cpp") == 0 ) {                       /* o */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* N */
  /*                             -cpp                             */     /* o */
  /*            Run the C preprocessor on SISAL files             */     /* t */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* C */
      nocpp = FALSE;                                                     /* h */
                                                                         /* a */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"cray") == 0 ) {                      /* g */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                            -cray                             */     /* M */
  /*                    Cross compile for Cray                    */     /* a */
  /* ------------------------------------------------------------ */     /* c */
                                                                         /* h */
      novec = FALSE;                                                     /* i */
      cvector = TRUE;                                                    /* n */
      cRay = TRUE;                                                       /* e */
      movereads = TRUE;                                                  /*   */
      chains = TRUE;                                                     /* G */
      newchains = TRUE;                                                  /* e */
      stopC = TRUE;                                                      /* n */
                                                                         /* e */
      break;                                                             /* r */
    } else if ( strcmp(argv[idx]+1,"cvector") == 0 ) {                   /* a */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /* e */
  /*                           -cvector                           */     /* d */
  /*                Get vectorization information                 */     /*   */
  /* ------------------------------------------------------------ */     /* D */
                                                                         /* o */
      novec = FALSE;                                                     /*   */
      cvector = TRUE;                                                    /* N */
      movereads = TRUE;                                                  /* o */
      chains = TRUE;                                                     /* t */
      newchains = TRUE;                                                  /*   */
                                                                         /* C */
      break;                                                             /* h */
    } else if ( strcmp(argv[idx]+1,"cvinfo") == 0 ) {                    /* a */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* g */
  /*                           -cvinfo                            */     /* e */
  /*      Get vectorization and concurentization information      */     /*   */
  /* ------------------------------------------------------------ */     /* M */
                                                                         /* a */
      cinfo |= ParseMap("1");                                            /* c */
      vinfo |= ParseMap("1");                                            /* h */
                                                                         /* i */
      break;                                                             /* n */
    } else if ( strncmp(argv[idx]+1,"cc=",3) == 0 ) {                    /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* G */
  /*                       -cc=<directive>                        */     /* e */
  /*             Supply <directive> to the C compiler             */     /* n */
  /* ------------------------------------------------------------ */     /* e */
    CorrectUsage = "Usage: -cc=<directive>";                             /* r */
    if ( argv[idx][3] == NULL ) goto OptionError;                        /* a */
                                                                         /* t */
      ccoptions[++ccoptionscnt] = &argv[idx][4];                         /* e */
                                                                         /* d */
      break;                                                             /*   */
    } else if ( strncmp(argv[idx]+1,"cvinfo=",7) == 0 ) {                /* D */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /*   */
  /*                     -cvinfo=<rangelist>                      */     /* N */
  /*   Get more vectorization and concurentization information    */     /* o */
  /* ------------------------------------------------------------ */     /* t */
    CorrectUsage = "Usage: -cvinfo=<rangelist>";                         /*   */
    if ( argv[idx][7] == NULL ) goto OptionError;                        /* C */
                                                                         /* h */
      cinfo |= ParseMap(argv[idx]+8);                                    /* a */
      vinfo |= ParseMap(argv[idx]+8);                                    /* n */
                                                                         /* g */
      break;                                                             /* e */
    }                                                                    /*   */
    goto OptionError;                                                    /* M */
                                                                         /* a */
   case 'd':                                                             /* c */
    if ( strcmp(argv[idx]+1,"d") == 0 ) {                                /* h */
                                                                         /* i */
  /* ------------------------------------------------------------ */     /* n */
  /*                              -d                              */     /* e */
  /*                          Debug mode                          */     /*   */
  /* ------------------------------------------------------------ */     /* G */
                                                                         /* e */
      debug = TRUE;                                                      /* n */
                                                                         /* e */
      break;                                                             /* r */
    } else if ( strcmp(argv[idx]+1,"db") == 0 ) {                        /* a */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /* e */
  /*                        -db <database>                        */     /* d */
  /*        Use <database> as inter-module data repository        */     /*   */
  /* ------------------------------------------------------------ */     /* D */
    CorrectUsage = "Usage: -db <database>";                              /* o */
    if ( (++idx) >= argc ) goto OptionError;                             /*   */
                                                                         /* N */
      mdb = argv[idx];                                                   /* o */
                                                                         /* t */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"dfuse") == 0 ) {                     /* C */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* a */
  /*                            -dfuse                            */     /* n */
  /*                Preform dependent loop fusion                 */     /* g */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /*   */
      nodfuse = FALSE;                                                   /* M */
                                                                         /* a */
      break;                                                             /* c */
    } else if ( strcmp(argv[idx]+1,"double_real") == 0 ) {               /* h */
                                                                         /* i */
  /* ------------------------------------------------------------ */     /* n */
  /*                         -double_real                         */     /* e */
  /*        Treat all SISAL real data as double_real data.        */     /*   */
  /* ------------------------------------------------------------ */     /* G */
                                                                         /* e */
      dbl = TRUE;                                                        /* n */
                                                                         /* e */
      break;                                                             /* r */
    }                                                                    /* a */
    goto OptionError;                                                    /* t */
                                                                         /* e */
   case 'e':                                                             /* d */
    if ( strcmp(argv[idx]+1,"e") == 0 ) {                                /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                          -e <funct>                          */     /*   */
  /*               Use <funct> as main entry point                */     /* N */
  /* ------------------------------------------------------------ */     /* o */
    CorrectUsage = "Usage: -e <funct>";                                  /* t */
    if ( (++idx) >= argc ) goto OptionError;                             /*   */
                                                                         /* C */
      eoptions[++eoptioncnt] = argv[idx];                                /* h */
                                                                         /* a */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"explode") == 0 ) {                   /* g */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -explode                           */     /* M */
  /*                  Aggressively explode loops                  */     /* a */
  /* ------------------------------------------------------------ */     /* c */
                                                                         /* h */
      explode = TRUE;                                                    /* i */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"explodeI") == 0 ) {                  /*   */
                                                                         /* G */
  /* ------------------------------------------------------------ */     /* e */
  /*                          -explodeI                           */     /* n */
  /*             Aggressively explode innermost loops             */     /* e */
  /* ------------------------------------------------------------ */     /* r */
                                                                         /* a */
      explode  = TRUE;                                                   /* t */
      explodeI = TRUE;                                                   /* e */
                                                                         /* d */
      break;                                                             /*   */
    }                                                                    /* D */
    goto OptionError;                                                    /* o */
                                                                         /*   */
   case 'f':                                                             /* N */
    if ( strcmp(argv[idx]+1,"f") == 0 ) {                                /* o */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /*   */
  /*                          -f <funct>                          */     /* C */
  /*       Consider <funct> available as a FORTRAN external       */     /* h */
  /* ------------------------------------------------------------ */     /* a */
    CorrectUsage = "Usage: -f <funct>";                                  /* n */
    if ( (++idx) >= argc ) goto OptionError;                             /* g */
                                                                         /* e */
      foptions[++foptioncnt] = argv[idx];                                /*   */
                                                                         /* M */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"fflopinfo") == 0 ) {                 /* c */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* i */
  /*                      -fflopinfo <funct>                      */     /* n */
  /*       Write floating point counts in <funct> to stderr       */     /* e */
  /* ------------------------------------------------------------ */     /*   */
    CorrectUsage = "Usage: -fflopinfo <funct>";                          /* G */
    if ( (++idx) >= argc ) goto OptionError;                             /* e */
                                                                         /* n */
      calloptions[++calloptioncnt] = argv[idx];                          /* e */
      flopoptions[++flopoptioncnt] = argv[idx];                          /* r */
      flpinfo = TRUE;                                                    /* a */
                                                                         /* t */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"flopinfo") == 0 ) {                  /* d */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* D */
  /*                          -flopinfo                           */     /* o */
  /*          Write all floating point counts to s.info           */     /*   */
  /* ------------------------------------------------------------ */     /* N */
                                                                         /* o */
      flpinfo = TRUE;                                                    /* t */
                                                                         /*   */
      break;                                                             /* C */
    } else if ( strcmp(argv[idx]+1,"forC") == 0 ) {                      /* h */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* n */
  /*                            -forC                             */     /* g */
  /*             Compile into a program callable by C             */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* M */
      forC = TRUE;                                                       /* a */
                                                                         /* c */
      break;                                                             /* h */
    } else if ( strcmp(argv[idx]+1,"forFORTRAN") == 0 ) {                /* i */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                         -forFORTRAN                          */     /*   */
  /*          Compile into a program callable by FORTRAN          */     /* G */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* n */
      forF = TRUE;                                                       /* e */
                                                                         /* r */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"freeall") == 0 ) {                   /* t */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* d */
  /*                           -freeall                           */     /*   */
  /*                 Force release of all storage                 */     /* D */
  /* ------------------------------------------------------------ */     /* o */
                                                                         /*   */
      freeall = TRUE;                                                    /* N */
                                                                         /* o */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"fuse") == 0 ) {                      /*   */
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                            -fuse                             */     /* a */
  /*                     Perform loop fusion                      */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      noifuse  = FALSE;                                                  /*   */
      nodfuse = FALSE;                                                   /* M */
                                                                         /* a */
      break;                                                             /* c */
    } else if ( strncmp(argv[idx]+1,"ff=",3) == 0 ) {                    /* h */
                                                                         /* i */
  /* ------------------------------------------------------------ */     /* n */
  /*                       -ff=<directive>                        */     /* e */
  /*        Give <directive> to the local FORTRAN compiler        */     /*   */
  /* ------------------------------------------------------------ */     /* G */
    CorrectUsage = "Usage: -ff=<directive>";                             /* e */
    if ( argv[idx][3] == NULL ) goto OptionError;                        /* n */
                                                                         /* e */
      ffoptions[++ffoptionscnt] = &argv[idx][4];                         /* r */
                                                                         /* a */
      break;                                                             /* t */
    }                                                                    /* e */
    goto OptionError;                                                    /* d */
                                                                         /*   */
   case 'g':                                                             /* D */
    if ( strcmp(argv[idx]+1,"glue") == 0 ) {                             /* o */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* N */
  /*                            -glue                             */     /* o */
  /*    Disable the optimization of non-inlined function calls    */     /* t */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* C */
      SISglue = TRUE;                                                    /* h */
                                                                         /* a */
      break;                                                             /* n */
    }                                                                    /* g */
    goto OptionError;                                                    /* e */
                                                                         /*   */
   case 'h':                                                             /* M */
    if ( strcmp(argv[idx]+1,"help") == 0 ) {                             /* a */
                                                                         /* c */
  /* ------------------------------------------------------------ */     /* h */
  /*                            -help                             */     /* i */
  /*                        Same as -usage                        */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /*   */
      PrintUsageTable(argv,idx);                                         /* G */
                                                                         /* e */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"hybrid") == 0 ) {                    /* e */
                                                                         /* r */
  /* ------------------------------------------------------------ */     /* a */
  /*                           -hybrid                            */     /* t */
  /*               Generate both C and FORTRAN code               */     /* e */
  /* ------------------------------------------------------------ */     /* d */
                                                                         /*   */
      hybrid = TRUE;                                                     /* D */
                                                                         /* o */
      break;                                                             /*   */
    } else if ( isdigit(argv[idx][2]) ) {                                /* N */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /* t */
  /*                           -h<num>                            */     /*   */
  /*        Slice loops only if cost is greater than <num>        */     /* C */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* a */
      huge = argv[idx];                                                  /* n */
      huge[1] = 'H';                                                     /* g */
                                                                         /* e */
      break;                                                             /*   */
    }                                                                    /* M */
    goto OptionError;                                                    /* a */
                                                                         /* c */
   case 'i':                                                             /* h */
    if ( strcmp(argv[idx]+1,"icse") == 0 ) {                             /* i */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                            -icse                             */     /*   */
  /*        Push identical operations out of conditionals         */     /* G */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* n */
      noaggressive = FALSE;                                              /* e */
                                                                         /* r */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"info") == 0 ) {                      /* t */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* d */
  /*                            -info                             */     /*   */
  /*                Produce an information listing                */     /* D */
  /* ------------------------------------------------------------ */     /* o */
                                                                         /*   */
      info |= ParseMap("1");                                             /* N */
                                                                         /* o */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"inlineall") == 0 ) {                 /*   */
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                          -inlineall                          */     /* a */
  /*                     Inline all functions                     */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      inlineall = TRUE;                                                  /*   */
                                                                         /* M */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"inter") == 0 ) {                     /* c */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* i */
  /*                            -inter                            */     /* n */
  /*         Interactively select functions for inlining          */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* G */
      inter = TRUE;                                                      /* e */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( isdigit(argv[idx][2]) ) {                                /* r */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* t */
  /*                           -i<num>                            */     /* e */
  /*            Set assumed iteration count for loops             */     /* d */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* D */
      iter = argv[idx];                                                  /* o */
      iter[1] = '@';                                                     /*   */
                                                                         /* N */
      break;                                                             /* o */
    } else if ( strncmp(argv[idx]+1,"info=",5) == 0 ) {                  /* t */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* C */
  /*                      -info=<rangelist>                       */     /* h */
  /*                     Get more information                     */     /* a */
  /* ------------------------------------------------------------ */     /* n */
    CorrectUsage = "Usage: -info=<rangelist>";                           /* g */
    if ( argv[idx][5] == NULL ) goto OptionError;                        /* e */
                                                                         /*   */
      info |= ParseMap(argv[idx]+6);                                     /* M */
                                                                         /* a */
      break;                                                             /* c */
    }                                                                    /* h */
    goto OptionError;                                                    /* i */
                                                                         /* n */
   case 'l':                                                             /* e */
    if ( strcmp(argv[idx]+1,"listing") == 0 ) {                          /*   */
                                                                         /* G */
  /* ------------------------------------------------------------ */     /* e */
  /*                           -listing                           */     /* n */
  /*        Produce a root.lst file for each root.sis file        */     /* e */
  /* ------------------------------------------------------------ */     /* r */
                                                                         /* a */
      list = TRUE;                                                       /* t */
                                                                         /* e */
      break;                                                             /* d */
    } else {                                                             /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                         -l<library>                          */     /*   */
  /*                     Add a loader library                     */     /* N */
  /* ------------------------------------------------------------ */     /* o */
    CorrectUsage = "Usage: -l<library>";                                 /* t */
    if ( argv[idx][1] == NULL ) goto OptionError;                        /*   */
                                                                         /* C */
      ld[++ldcnt] = argv[idx];                                           /* h */
                                                                         /* a */
      break;                                                             /* n */
    }                                                                    /* g */
                                                                         /* e */
   case 'm':                                                             /*   */
    if ( strcmp(argv[idx]+1,"makeloopreport") == 0 ) {                   /* M */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* c */
  /*                    -makeloopreport <file>                    */     /* h */
  /*   Create a loop report showing the partitioner's decisions   */     /* i */
  /* ------------------------------------------------------------ */     /* n */
    CorrectUsage = "Usage: -makeloopreport <file>";                      /* e */
    if ( (++idx) >= argc ) goto OptionError;                             /*   */
                                                                         /* G */
      LoopReportOut = argv[idx];                                         /* e */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"maxconcur") == 0 ) {                 /* r */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* t */
  /*                          -maxconcur                          */     /* e */
  /*    Disable cost estimate.  Parallelize all eligible loops    */     /* d */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* D */
      concur = TRUE;                                                     /* o */
      huge  = "-H1.0";                                                   /*   */
      procs = "-P4000000";                                               /* N */
                                                                         /* o */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"module") == 0 ) {                    /*   */
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                           -module                            */     /* a */
  /*             Compile files into a callable module             */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      smodule = TRUE;                                                    /*   */
                                                                         /* M */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"movereads") == 0 ) {                 /* c */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* i */
  /*                          -movereads                          */     /* n */
  /*                 Move read operations (Cray)                  */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* G */
      movereads = TRUE;                                                  /* e */
                                                                         /* n */
      break;                                                             /* e */
    }                                                                    /* r */
    goto OptionError;                                                    /* a */
                                                                         /* t */
   case 'n':                                                             /* e */
    if ( strcmp(argv[idx]+1,"nancy") == 0 ) {                            /* d */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* D */
  /*                            -nancy                            */     /* o */
  /*        Do not use original Cray microtasking software        */     /*   */
  /* ------------------------------------------------------------ */     /* N */
                                                                         /* o */
      useORTS = FALSE;                                                   /* t */
                                                                         /*   */
      break;                                                             /* C */
    } else if ( strcmp(argv[idx]+1,"newchains") == 0 ) {                 /* h */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* n */
  /*                          -newchains                          */     /* g */
  /*                    Form Cray X-MP Chains                     */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* M */
      newchains = TRUE;                                                  /* a */
                                                                         /* c */
      break;                                                             /* h */
    } else if ( strcmp(argv[idx]+1,"nltss") == 0 ) {                     /* i */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                            -nltss                            */     /*   */
  /*            Use NLTSS pragmas to generate vectors             */     /* G */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* n */
      nltss = TRUE;                                                      /* e */
                                                                         /* r */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"noOinvar") == 0 ) {                  /* t */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* d */
  /*                          -noOinvar                           */     /*   */
  /*          Do not remove invariants from inner loops           */     /* D */
  /* ------------------------------------------------------------ */     /* o */
                                                                         /*   */
      noOinvar = TRUE;                                                   /* N */
                                                                         /* o */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"noaimp") == 0 ) {                    /*   */
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                           -noaimp                            */     /* a */
  /*              Do not optimize array dereferences              */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      noaimp = TRUE;                                                     /*   */
                                                                         /* M */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"noamove") == 0 ) {                   /* c */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* i */
  /*                           -noamove                           */     /* n */
  /*   Do not apply anti-movement optimization (from compounds)   */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* G */
      noamove = TRUE;                                                    /* e */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"noassoc") == 0 ) {                   /* r */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* t */
  /*                           -noassoc                           */     /* e */
  /*     Disable parallel reduction of associative operations     */     /* d */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* D */
      noassoc = TRUE;                                                    /* o */
                                                                         /*   */
      break;                                                             /* N */
    } else if ( strcmp(argv[idx]+1,"nobip") == 0 ) {                     /* o */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /*   */
  /*                            -nobip                            */     /* C */
  /*                         Disable BIP                          */     /* h */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* n */
      nobip = TRUE;                                                      /* g */
                                                                         /* e */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"nobipmv") == 0 ) {                   /* M */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* c */
  /*                           -nobipmv                           */     /* h */
  /*       Do not allow buffer movement in the C generator        */     /* i */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      bipmv = FALSE;                                                     /*   */
                                                                         /* G */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"nobounds") == 0 ) {                  /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* r */
  /*                          -nobounds                           */     /* a */
  /*       Do not generate code to check for various errors       */     /* t */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* d */
      bounds = FALSE;                                                    /*   */
                                                                         /* D */
      break;                                                             /* o */
    } else if ( strcmp(argv[idx]+1,"nobrec") == 0 ) {                    /*   */
                                                                         /* N */
  /* ------------------------------------------------------------ */     /* o */
  /*                           -nobrec                            */     /* t */
  /*              Disable basic record optimization               */     /*   */
  /* ------------------------------------------------------------ */     /* C */
                                                                         /* h */
      nobrec = TRUE;                                                     /* a */
                                                                         /* n */
      break;                                                             /* g */
    } else if ( strcmp(argv[idx]+1,"nocagg") == 0 ) {                    /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* M */
  /*                           -nocagg                            */     /* a */
  /*     Do not mark constant aggregates for static building      */     /* c */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* i */
      nocagg = TRUE;                                                     /* n */
                                                                         /* e */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"nochains") == 0 ) {                  /* G */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* n */
  /*                          -nochains                           */     /* e */
  /*                   Disable vector chaining                    */     /* r */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* t */
      chains    = FALSE;                                                 /* e */
      newchains = FALSE;                                                 /* d */
                                                                         /*   */
      break;                                                             /* D */
    } else if ( strcmp(argv[idx]+1,"nocom") == 0 ) {                     /* o */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* N */
  /*                            -nocom                            */     /* o */
  /*          Disable Sequent code improvement migration          */     /* t */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* C */
      nocom = TRUE;                                                      /* h */
                                                                         /* a */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"noconcur") == 0 ) {                  /* g */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                          -noconcur                           */     /* M */
  /*                Disable concurrent processing                 */     /* a */
  /* ------------------------------------------------------------ */     /* c */
                                                                         /* h */
      concur = FALSE;                                                    /* i */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"nocpp") == 0 ) {                     /*   */
                                                                         /* G */
  /* ------------------------------------------------------------ */     /* e */
  /*                            -nocpp                            */     /* n */
  /*         Do not run the C preprocessor on SISAL files         */     /* e */
  /* ------------------------------------------------------------ */     /* r */
                                                                         /* a */
      nocpp = TRUE;                                                      /* t */
                                                                         /* e */
      break;                                                             /* d */
    } else if ( strcmp(argv[idx]+1,"nocse") == 0 ) {                     /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                            -nocse                            */     /*   */
  /*               Disable common subr. eliminator                */     /* N */
  /* ------------------------------------------------------------ */     /* o */
                                                                         /* t */
      nocse = TRUE;                                                      /*   */
                                                                         /* C */
      break;                                                             /* h */
    } else if ( strcmp(argv[idx]+1,"nodead") == 0 ) {                    /* a */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* g */
  /*                           -nodead                            */     /* e */
  /*              No dead code removal in optimizer               */     /*   */
  /* ------------------------------------------------------------ */     /* M */
                                                                         /* a */
      dead = FALSE;                                                      /* c */
                                                                         /* h */
      break;                                                             /* i */
    } else if ( strcmp(argv[idx]+1,"nodfuse") == 0 ) {                   /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -nodfuse                           */     /* G */
  /*                Disable dependent loop fusion                 */     /* e */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      nodfuse = TRUE;                                                    /* r */
                                                                         /* a */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"nodope") == 0 ) {                    /* e */
                                                                         /* d */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -nodope                            */     /* D */
  /*            Do not apply dope vector optimizations            */     /* o */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* N */
      nodope = TRUE;                                                     /* o */
                                                                         /* t */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"nofcopy") == 0 ) {                   /* C */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* a */
  /*                           -nofcopy                           */     /* n */
  /*                           Unknown?                           */     /* g */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /*   */
      nofcopy = TRUE;                                                    /* M */
                                                                         /* a */
      break;                                                             /* c */
    } else if ( strcmp(argv[idx]+1,"nofiss") == 0 ) {                    /* h */
                                                                         /* i */
  /* ------------------------------------------------------------ */     /* n */
  /*                           -nofiss                            */     /* e */
  /*                Do not attempt record fission                 */     /*   */
  /* ------------------------------------------------------------ */     /* G */
                                                                         /* e */
      norecf = TRUE;                                                     /* n */
                                                                         /* e */
      break;                                                             /* r */
    } else if ( strcmp(argv[idx]+1,"nofold") == 0 ) {                    /* a */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /* e */
  /*                           -nofold                            */     /* d */
  /*            Do not attempt to fold constant values            */     /*   */
  /* ------------------------------------------------------------ */     /* D */
                                                                         /* o */
      nofold = TRUE;                                                     /*   */
                                                                         /* N */
      break;                                                             /* o */
    } else if ( strcmp(argv[idx]+1,"nofuse") == 0 ) {                    /* t */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* C */
  /*                           -nofuse                            */     /* h */
  /*                   Disable all loop fusion                    */     /* a */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* g */
      noifuse = TRUE;                                                    /* e */
      nodfuse = TRUE;                                                    /*   */
                                                                         /* M */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"nogcse") == 0 ) {                    /* c */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* i */
  /*                           -nogcse                            */     /* n */
  /*        Do not attempt global common subr. elimination        */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* G */
      nogcse = TRUE;                                                     /* e */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"noif1opt") == 0 ) {                  /* r */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* t */
  /*                          -noif1opt                           */     /* e */
  /*                 Turn off IF1 code improvers                  */     /* d */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* D */
      noif1opt = TRUE;                                                   /* o */
                                                                         /*   */
      break;                                                             /* N */
    } else if ( strcmp(argv[idx]+1,"noifuse") == 0 ) {                   /* o */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -noifuse                           */     /* C */
  /*               Disable independent loop fusion                */     /* h */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* n */
      noifuse = TRUE;                                                    /* g */
                                                                         /* e */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"noimp") == 0 ) {                     /* M */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* c */
  /*                            -noimp                            */     /* h */
  /*           Set -nvnoopt loader option for the Crays           */     /* i */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      noimp = TRUE;                                                      /*   */
                                                                         /* G */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"noimp") == 0 ) {                     /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* r */
  /*                            -noimp                            */     /* a */
  /*      Compile with the C compiler's optimizers disabled       */     /* t */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* d */
      noimp = TRUE;                                                      /*   */
                                                                         /* D */
      break;                                                             /* o */
    } else if ( strcmp(argv[idx]+1,"noinline") == 0 ) {                  /*   */
                                                                         /* N */
  /* ------------------------------------------------------------ */     /* o */
  /*                          -noinline                           */     /* t */
  /*                   Do not inline functions                    */     /*   */
  /* ------------------------------------------------------------ */     /* C */
                                                                         /* h */
      noinline = TRUE;                                                   /* a */
                                                                         /* n */
      break;                                                             /* g */
    } else if ( strcmp(argv[idx]+1,"noinvar") == 0 ) {                   /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* M */
  /*                           -noinvar                           */     /* a */
  /*             Disable invariant removal optimizer              */     /* c */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* i */
      noinvar = TRUE;                                                    /* n */
                                                                         /* e */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"noinvert") == 0 ) {                  /* G */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* n */
  /*                          -noinvert                           */     /* e */
  /*          Do not perform loop inversion optimization          */     /* r */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* t */
      noinvert = TRUE;                                                   /* e */
                                                                         /* d */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"noload") == 0 ) {                    /* D */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -noload                            */     /* N */
  /*      Do not create the executable, stop with a .o file       */     /* o */
  /* ------------------------------------------------------------ */     /* t */
                                                                         /*   */
      stopc = TRUE;                                                      /* C */
      TMPDIR="";                                                         /* h */
                                                                         /* a */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"nomem") == 0 ) {                     /* g */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                            -nomem                            */     /* M */
  /*               Disable build in place analysis                */     /* a */
  /* ------------------------------------------------------------ */     /* c */
                                                                         /* h */
      noif2mem = TRUE;                                                   /* i */
      preb     = "-Y0";                                                  /* n */
                                                                         /* e */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"nomig") == 0 ) {                     /* G */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* n */
  /*                            -nomig                            */     /* e */
  /*            Do not migrate operations toward users            */     /* r */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* t */
      nomig = TRUE;                                                      /* e */
                                                                         /* d */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"nomovereads") == 0 ) {               /* D */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /*   */
  /*                         -nomovereads                         */     /* N */
  /*                  Move array read operations                  */     /* o */
  /* ------------------------------------------------------------ */     /* t */
                                                                         /*   */
      movereads = FALSE;                                                 /* C */
                                                                         /* h */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"nonormidx") == 0 ) {                 /* n */
                                                                         /* g */
  /* ------------------------------------------------------------ */     /* e */
  /*                          -nonormidx                          */     /*   */
  /*               Do not normalize array indexing                */     /* M */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* c */
      nonormidx = TRUE;                                                  /* h */
                                                                         /* i */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"noopt") == 0 ) {                     /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* G */
  /*                            -noopt                            */     /* e */
  /*                   Disable all optimization                   */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* r */
      noopt = TRUE;                                                      /* a */
      noimp = TRUE;                                                      /* t */
      bounds = FALSE;                                                    /* e */
                                                                         /* d */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"nopreb") == 0 ) {                    /* D */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -nopreb                            */     /* N */
  /*                   Disable array prebuilds                    */     /* o */
  /* ------------------------------------------------------------ */     /* t */
                                                                         /*   */
      preb = "-Y0";                                                      /* C */
                                                                         /* h */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"norag") == 0 ) {                     /* n */
                                                                         /* g */
  /* ------------------------------------------------------------ */     /* e */
  /*                            -norag                            */     /*   */
  /*               Generate code called by FORTRAN                */     /* M */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* c */
      norag = TRUE;                                                      /* h */
                                                                         /* i */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"noregs") == 0 ) {                    /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* G */
  /*                           -noregs                            */     /* e */
  /*     Do not assign register prefixes to generated C code      */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* r */
      noregs = TRUE;                                                     /* a */
                                                                         /* t */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"noscalar") == 0 ) {                  /* d */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* D */
  /*                          -noscalar                           */     /* o */
  /*                 Disable scalar optimizations                 */     /*   */
  /* ------------------------------------------------------------ */     /* N */
                                                                         /* o */
      noscalar = TRUE;                                                   /* t */
                                                                         /*   */
      break;                                                             /* C */
    } else if ( strcmp(argv[idx]+1,"nosfuse") == 0 ) {                   /* h */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* n */
  /*                           -nosfuse                           */     /* g */
  /*                    Disable select fusion                     */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* M */
      nosfuse = TRUE;                                                    /* a */
                                                                         /* c */
      break;                                                             /* h */
    } else if ( strcmp(argv[idx]+1,"nosplit") == 0 ) {                   /* i */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                           -nosplit                           */     /*   */
  /*                Do not preform loop splitting                 */     /* G */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* n */
      nosplit = TRUE;                                                    /* e */
                                                                         /* r */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"nostr") == 0 ) {                     /* t */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* d */
  /*                            -nostr                            */     /*   */
  /*      Do not attempt to identify single threaded streams      */     /* D */
  /* ------------------------------------------------------------ */     /* o */
                                                                         /*   */
      nostr = TRUE;                                                      /* N */
                                                                         /* o */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"nostrip") == 0 ) {                   /*   */
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                           -nostrip                           */     /* a */
  /*             Do not perform return node stripping             */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      nostrip = TRUE;                                                    /*   */
                                                                         /* M */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"notgcse") == 0 ) {                   /* c */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* i */
  /*                           -notgcse                           */     /* n */
  /*         Do not force global common subr. elimination         */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* G */
      notgcse = TRUE;                                                    /* e */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"nounroll") == 0 ) {                  /* r */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* t */
  /*                          -nounroll                           */     /* e */
  /*                    Disable loop unrolling                    */     /* d */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* D */
      unroll = "-U0";                                                    /* o */
                                                                         /*   */
      break;                                                             /* N */
    } else if ( strcmp(argv[idx]+1,"noup") == 0 ) {                      /* o */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /*   */
  /*                            -noup                             */     /* C */
  /*               Disable update in place analysis               */     /* h */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* n */
      noif2up = TRUE;                                                    /* g */
      preb  = "-Y0";                                                     /* e */
                                                                         /*   */
      break;                                                             /* M */
    } else if ( strcmp(argv[idx]+1,"novector") == 0 ) {                  /* a */
                                                                         /* c */
  /* ------------------------------------------------------------ */     /* h */
  /*                          -novector                           */     /* i */
  /*                    Disable vectorization                     */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /*   */
      novec = TRUE;                                                      /* G */
                                                                         /* e */
      break;                                                             /* n */
    } else if ( strcmp(argv[idx]+1,"nqs") == 0 ) {                       /* e */
                                                                         /* r */
  /* ------------------------------------------------------------ */     /* a */
  /*                             -nqs                             */     /* t */
  /*       ...options  Submit as NQS batch job (Cray only)        */     /* e */
  /* ------------------------------------------------------------ */     /* d */
                                                                         /*   */
      SubmitNQS(argc,argv,idx);                                          /* D */
                                                                         /* o */
      break;                                                             /*   */
    } else if ( isdigit(argv[idx][2]) ) {                                /* N */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /* t */
  /*                           -n<num>                            */     /*   */
  /*    Consider only loops nested <num> deep for concurrency     */     /* C */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* a */
      level = argv[idx];                                                 /* n */
      level[1] = 'L';                                                    /* g */
                                                                         /* e */
      break;                                                             /*   */
    }                                                                    /* M */
    goto OptionError;                                                    /* a */
                                                                         /* c */
   case 'o':                                                             /* h */
    if ( strcmp(argv[idx]+1,"o") == 0 ) {                                /* i */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                        -o <absolute>                         */     /*   */
  /*       Name the executable <absolute> instead of s.out        */     /* G */
  /* ------------------------------------------------------------ */     /* e */
    CorrectUsage = "Usage: -o <absolute>";                               /* n */
    if ( (++idx) >= argc ) goto OptionError;                             /* e */
                                                                         /* r */
      aabs = argv[idx];                                                  /* a */
                                                                         /* t */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"oo") == 0 ) {                        /* d */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* D */
  /*                        -oo <absolute>                        */     /* o */
  /*        Just like -o, but sisal extensions are legal.         */     /*   */
  /* ------------------------------------------------------------ */     /* N */
    CorrectUsage = "Usage: -oo <absolute>";                              /* o */
    if ( (++idx) >= argc ) goto OptionError;                             /* t */
                                                                         /*   */
      NameSafety = FALSE;                                                /* C */
      aabs = argv[idx];                                                  /* h */
                                                                         /* a */
      break;                                                             /* n */
    }                                                                    /* g */
    goto OptionError;                                                    /* e */
                                                                         /*   */
   case 'p':                                                             /* M */
    if ( strcmp(argv[idx]+1,"prof") == 0 ) {                             /* a */
                                                                         /* c */
  /* ------------------------------------------------------------ */     /* h */
  /*                            -prof                             */     /* i */
  /*      Generate a time execution profile of the compiler       */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /*   */
      prof = TRUE;                                                       /* G */
      verbose = TRUE;                                                    /* e */
                                                                         /* n */
      break;                                                             /* e */
    } else if ( strcmp(argv[idx]+1,"progress") == 0 ) {                  /* r */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* t */
  /*                          -progress                           */     /* e */
  /*        Generate progress report in C code generation         */     /* d */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* D */
      prog = TRUE;                                                       /* o */
                                                                         /*   */
      break;                                                             /* N */
    } else if ( isdigit(argv[idx][2]) ) {                                /* o */
                                                                         /* t */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -p<num>                            */     /* C */
  /*            Assume <num> processors for partioning            */     /* h */
  /* ------------------------------------------------------------ */     /* a */
                                                                         /* n */
      procs = argv[idx];                                                 /* g */
      procs[1] = 'P';                                                    /* e */
                                                                         /*   */
      break;                                                             /* M */
      } else if ( strncmp(argv[idx]+1,"patch=",6) == 0 && isdigit(argv[idx][7]) ) {
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* c */
  /*                         -patch=<num>                         */     /* h */
  /*   Apply a dynamic patch to one of the backend applications   */     /* i */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      if ( PatchCount >= sizeof(Patches)/sizeof(Patches[0]) ) {          /*   */
        Error2( argv[idx] , "-- Too many patches" );                     /* G */
      }                                                                  /* e */
                                                                         /* n */
      Patches[PatchCount] = (char *) malloc(strlen(argv[idx]));          /* e */
      sprintf(Patches[PatchCount],"-p%s",argv[idx]+7);                   /* r */
      PatchCount++;                                                      /* a */
                                                                         /* t */
      break;                                                             /* e */
    } else if ( argv[idx][2] == 'b' && isdigit(argv[idx][3]) ) {         /* d */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* D */
  /*                           -pb<num>                           */     /* o */
  /*            Set array prebuild dimension to <num>             */     /*   */
  /* ------------------------------------------------------------ */     /* N */
                                                                         /* o */
      preb = &(argv[idx][1]);                                            /* t */
      preb[0] = '-';                                                     /*   */
      preb[1] = 'Y';                                                     /* C */
                                                                         /* h */
      break;                                                             /* a */
    }                                                                    /* n */
    goto OptionError;                                                    /* g */
                                                                         /* e */
   case 'r':                                                             /*   */
    if ( strcmp(argv[idx]+1,"real") == 0 ) {                             /* M */
                                                                         /* a */
  /* ------------------------------------------------------------ */     /* c */
  /*                            -real                             */     /* h */
  /*        Treat all SISAL double_real data as real data.        */     /* i */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      flt = TRUE;                                                        /*   */
                                                                         /* G */
      break;                                                             /* e */
    }                                                                    /* n */
    goto OptionError;                                                    /* e */
                                                                         /* r */
   case 's':                                                             /* a */
    if ( strcmp(argv[idx]+1,"sdbx") == 0 ) {                             /* t */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /* d */
  /*                            -sdbx                             */     /*   */
  /*    Generate code to interface with the symbolic debugger     */     /* D */
  /* ------------------------------------------------------------ */     /* o */
                                                                         /*   */
      sdbx = TRUE;                                                       /* N */
      bounds = FALSE;   /* The TRUE default was interfering with other checks */
                                                                         /* o */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"seq") == 0 ) {                       /*   */
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                             -seq                             */     /* a */
  /*               Compile for sequential execution               */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      concur = FALSE;                                                    /*   */
      novec = TRUE;                                                      /* M */
                                                                         /* a */
      break;                                                             /* c */
    }                                                                    /* h */
    goto OptionError;                                                    /* i */
                                                                         /* n */
   case 't':                                                             /* e */
    if ( strcmp(argv[idx]+1,"tgcse") == 0 ) {                            /*   */
                                                                         /* G */
  /* ------------------------------------------------------------ */     /* e */
  /*                            -tgcse                            */     /* n */
  /*            Force global Common Subr. Elimination             */     /* e */
  /* ------------------------------------------------------------ */     /* r */
                                                                         /* a */
      notgcse = FALSE;                                                   /* t */
                                                                         /* e */
      break;                                                             /* d */
    } else if ( strcmp(argv[idx]+1,"time") == 0 ) {                      /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                        -time <funct>                         */     /*   */
  /*           Generating timing code for this function           */     /* N */
  /* ------------------------------------------------------------ */     /* o */
    CorrectUsage = "Usage: -time <funct>";                               /* t */
    if ( (++idx) >= argc ) goto OptionError;                             /*   */
                                                                         /* C */
      calloptions[++calloptioncnt] = argv[idx];                          /* h */
      timeoptions[++timeoptioncnt] = argv[idx];                          /* a */
                                                                         /* n */
      break;                                                             /* g */
    } else if ( strcmp(argv[idx]+1,"timeall") == 0 ) {                   /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* M */
  /*                           -timeall                           */     /* a */
  /*            Generate timing code for all functions            */     /* c */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* i */
      timeall = TRUE;                                                    /* n */
      noinline = TRUE;                                                   /* e */
                                                                         /*   */
      break;                                                             /* G */
    } else if ( strcmp(argv[idx]+1,"tmpdir") == 0 ) {                    /* e */
                                                                         /* n */
  /* ------------------------------------------------------------ */     /* e */
  /*                      -tmpdir <dirname>                       */     /* r */
  /*           Put intermediate temp files in <dirname>           */     /* a */
  /* ------------------------------------------------------------ */     /* t */
    CorrectUsage = "Usage: -tmpdir <dirname>";                           /* e */
    if ( (++idx) >= argc ) goto OptionError;                             /* d */
                                                                         /*   */
      TMPDIR = argv[idx];                                                /* D */
                                                                         /* o */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"trace") == 0 ) {                     /* N */
                                                                         /* o */
  /* ------------------------------------------------------------ */     /* t */
  /*                        -trace <funct>                        */     /*   */
  /*          Generating tracing code for this function           */     /* C */
  /* ------------------------------------------------------------ */     /* h */
    CorrectUsage = "Usage: -trace <funct>";                              /* a */
    if ( (++idx) >= argc ) goto OptionError;                             /* n */
                                                                         /* g */
      calloptions[++calloptioncnt] = argv[idx];                          /* e */
      traceoptions[++traceoptioncnt] = argv[idx];                        /*   */
                                                                         /* M */
      break;                                                             /* a */
    } else if ( strcmp(argv[idx]+1,"traceall") == 0 ) {                  /* c */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* i */
  /*                          -traceall                           */     /* n */
  /*           Generate tracing code for all functions            */     /* e */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* G */
      traceall = TRUE;                                                   /* e */
      noinline = TRUE;                                                   /* n */
                                                                         /* e */
      break;                                                             /* r */
    }                                                                    /* a */
    goto OptionError;                                                    /* t */
                                                                         /* e */
   case 'u':                                                             /* d */
    if ( strcmp(argv[idx]+1,"usage") == 0 ) {                            /*   */
                                                                         /* D */
  /* ------------------------------------------------------------ */     /* o */
  /*                            -usage                            */     /*   */
  /*                      Produce this list                       */     /* N */
  /* ------------------------------------------------------------ */     /* o */
    PrintUsageTable(argv,idx);                                           /* t */
      break;                                                             /*   */
    } else if ( strcmp(argv[idx]+1,"useloopreport") == 0 ) {             /* C */
                                                                         /* h */
  /* ------------------------------------------------------------ */     /* a */
  /*                    -useloopreport <file>                     */     /* n */
  /*          Use loopreport from a previous compilation          */     /* g */
  /* ------------------------------------------------------------ */     /* e */
    CorrectUsage = "Usage: -useloopreport <file>";                       /*   */
    if ( (++idx) >= argc ) goto OptionError;                             /* M */
                                                                         /* a */
      LoopReportIn = argv[idx];                                          /* c */
                                                                         /* h */
      break;                                                             /* i */
    } else if ( isdigit(argv[idx][2]) ) {                                /* n */
                                                                         /* e */
  /* ------------------------------------------------------------ */     /*   */
  /*                           -u<num>                            */     /* G */
  /*         Unroll loops with less than <num> iterations         */     /* e */
  /* ------------------------------------------------------------ */     /* n */
                                                                         /* e */
      unroll = argv[idx];                                                /* r */
      unroll[1] = 'U';                                                   /* a */
                                                                         /* t */
      break;                                                             /* e */
    }                                                                    /* d */
    goto OptionError;                                                    /*   */
                                                                         /* D */
   case 'v':                                                             /* o */
    if ( strcmp(argv[idx]+1,"v") == 0 ) {                                /*   */
                                                                         /* N */
  /* ------------------------------------------------------------ */     /* o */
  /*                              -v                              */     /* t */
  /*                         Verbose mode                         */     /*   */
  /* ------------------------------------------------------------ */     /* C */
                                                                         /* h */
      verbose = TRUE;                                                    /* a */
                                                                         /* n */
      break;                                                             /* g */
    } else if ( strcmp(argv[idx]+1,"vector") == 0 ) {                    /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* M */
  /*                           -vector                            */     /* a */
  /*                     Enable vectorization                     */     /* c */
  /* ------------------------------------------------------------ */     /* h */
                                                                         /* i */
      novec = FALSE;                                                     /* n */
#ifndef CRAY                                                             /* e */
#ifndef ALLIANT                                                          /*   */
      cvector   = TRUE;             /* RIGHT NOW, THE CRAY IS THE DEFAULT */
      movereads = TRUE;                                                  /* G */
      chains    = TRUE;                                                  /* e */
      newchains = TRUE;                                                  /* n */
#endif                                                                   /* e */
#endif                                                                   /* r */
                                                                         /* a */
      break;                                                             /* t */
    } else if ( strcmp(argv[idx]+1,"vinfo") == 0 ) {                     /* e */
                                                                         /* d */
  /* ------------------------------------------------------------ */     /*   */
  /*                            -vinfo                            */     /* D */
  /*              Produce vectorization information               */     /* o */
  /* ------------------------------------------------------------ */     /*   */
                                                                         /* N */
      vinfo = TRUE;                                                      /* o */
                                                                         /* t */
      break;                                                             /*   */
      } else if ( strncmp(argv[idx]+1,"vinfo",5) == 0 && isdigit(argv[idx][6]) ) {
                                                                         /* C */
  /* ------------------------------------------------------------ */     /* h */
  /*                         -vinfo<num>                          */     /* a */
  /*            Produce more vectorization information            */     /* n */
  /* ------------------------------------------------------------ */     /* g */
                                                                         /* e */
      vinfo = atoi(argv[idx]+6);                                         /*   */
                                                                         /* M */
      break;                                                             /* a */
    }                                                                    /* c */
    goto OptionError;                                                    /* h */
                                                                         /* i */
   case 'w':                                                             /* n */
    if ( strcmp(argv[idx]+1,"w") == 0 ) {                                /* e */
                                                                         /*   */
  /* ------------------------------------------------------------ */     /* G */
  /*                              -w                              */     /* e */
  /*                   Disable warning messages                   */     /* n */
  /* ------------------------------------------------------------ */     /* e */
                                                                         /* r */
      Warnings = FALSE;                                                  /* a */
                                                                         /* t */
      break;                                                             /* e */
    }                                                                    /* d */
    goto OptionError;                                                    /*   */
                                                                         /* D */
   case 'x':                                                             /* o */
    if ( strcmp(argv[idx]+1,"xchains") == 0 ) {                          /*   */
                                                                         /* N */
  /* ------------------------------------------------------------ */     /* o */
  /*                           -xchains                           */     /* t */
  /*                    Allow vector chaining                     */     /*   */
  /* ------------------------------------------------------------ */     /* C */
                                                                         /* h */
      chains = TRUE;                                                     /* a */
                                                                         /* n */
      break;                                                             /* g */
    }                                                                    /* e */
    goto OptionError;                                                    /*   */
   default: goto OptionError;                                            /* M */
  }                                                                      /* a */
}                                                                        /* c */
