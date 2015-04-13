/* M */  char *Usage[] = {
/* a */  "-X                    All remaining arguments treated as FIBRE inputs",
/* c */  "-ax<num>              Set array expansion factor",
/* h */  "-b                    Use distributed run queue system to bind work to processes",
/* i */  "-bformat <formatstring> Change the default boolean output format",
/* n */  "-blocked              Use a blocked scheduling algorithm for loops (default)",
/* e */  "-cformat <formatstring> Change the default integer output format for printable characters",
/*   */  "-cformat2 <formatstring> Change the default uprintable character output format",
/* G */  "-dformat <formatstring> Change the default double real output format",
/* e */  "-ds<num>              Initialize the shared data management pool to <num> bytes.",
/* n */  "-dx<num>              Set the exact fit storage allocation threshold to <num> bytes.",
/* e */  "-fformat <formatstring> Change the default real  output format",
/* r */  "-gss                  Use guided self scheduling algorithm instead of block slicing",
/* a */  "-help                *Same as -usage",
/* t */  "-iformat <formatstring> Change the default integer output format",
/* e */  "-ls<num>              Set the default slice count to <num>",
/* d */  "-nb                  *Do not bind parallel work",
/*   */  "-nformat <formatstring> Change the default null type output format",
/* D */  "-nostrings            Do not print array[character] in string (quoted) format",
/* o */  "-nqs                  ...options  Submit as NQS batch job (Cray only)",
/*   */  "-r                    Append resource utilization information to the file s.info",
/* N */  "-strided              Use strided loop scheduling algorithm instead of block slicing",
/* o */  "-tr=<attribute>       Interface to tracer function",
/* t */  "-usage                Produce this list",
/*   */  "-w<num>               Set number of worker processes to <num>",
/* C */  "-z                    Do not print the program's output.",
/* h */   NULL };
/* a */  static void PrintUsageTable(argv,idx)
/* n */  char *argv[]; int idx;
/* g */  {
/* e */    char **p,*q,*r,buf[128],lbuf[128],c;
/*   */    int len,cnt=0;
/* M */    if (argv[idx+1]) {
/* a */       while(argv[++idx]) {
/* c */         len=0;q=argv[idx];r=buf;
/* h */         while(*q) {len++; c= *q++; if (isupper(c)) c=tolower(c);*r++=c;};
/* i */         *r=NULL;
/* n */        for (p=Usage;*p;p++) {
/* e */          q= *p;r=lbuf;
/*   */          while(*q) {c= *q++; if (isupper(c)) c=tolower(c); *r++=c;};
/* G */           *r=NULL;
/* e */          for(q=lbuf;*q;q++) if (strncmp(q,buf,len)==0) { (void)puts(*p); cnt++; break; }
/* n */        }
/* e */      }
/* r */      if (!cnt) puts("No matches");
/* a */    } else {
/* t */      for (p=Usage;*p;p++) (void)puts(*p);
/* e */    }
/* d */    exit(1);
/*   */  }
