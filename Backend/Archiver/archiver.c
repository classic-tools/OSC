#include "world.h"

typedef void    ((*Func)());

FILE	*input		= stdin;
FILE	*output		= stdout;
char	*program	= "archiver";

filenode	*filelist = NULL;
namelink	*GlobalHave = NULL;
int		MakeTmpFile = 0;


/**************************************************************************/
/* LOCAL  **************          SafeOpen         ************************/
/**************************************************************************/
/* PURPOSE:  Try to open a file. Report an error if it cannot open	  */
/**************************************************************************/
static FILE *SafeOpen(file,tag)
     char	*file;
     char	*tag;
{
  FILE		*fp;

  fp = fopen(file,tag);
  if ( !fp ) {
    perror(file);
    Error2("Cannot open",file);
  }

  return fp;
}

/**************************************************************************/
/* LOCAL  **************       CreateArchive       ************************/
/**************************************************************************/
/* PURPOSE:  Build an archive file and SYMDEF header from a list of files */
/**************************************************************************/
static void CreateArchive(argc,argv)
     int	argc;
     char	*argv[];
{
  int		idx;
  PNODE		F;
  filenode	*cur;
  namelink	*name;
  filenode	*P,*NP,*LP;
  char		linebuf[256];
  char		lowername[MAX_PATH];
  char		*p;

  /* ------------------------------------------------------------ */
  /* Create the archive file (stdout if -)			  */
  /* ------------------------------------------------------------ */
  if ( strcmp(argv[0],"-") != 0 ) output = SafeOpen(argv[0],"w");

  /* ------------------------------------------------------------ */
  /* For each file specified, read it in and build up a list of	  */
  /* Haves (functions exported) 				  */
  /* ------------------------------------------------------------ */
  for(idx=1; idx<argc; idx++) {
    input = SafeOpen(argv[idx],"r");

    cur = (filenode*)(malloc(sizeof(filenode)));
    cur->file = argv[idx];
    cur->have = (namelink*)NULL;
    cur->next = filelist;
    filelist = cur;
    If1Read();

    BuildHaveList(&cur->have);

    fclose(input);
    ReadItems();
    BuildItems();
  }

  /* ------------------------------------------------------------ */
  /* Reverse the elements of the file chain to get things in the  */
  /* right order						  */
  /* ------------------------------------------------------------ */
  for(LP=(filenode*)NULL,P=filelist; P; LP=P,P=NP) { 
    NP = P->next; 
    P->next = LP; 
  } 
  filelist = LP;

  /* ------------------------------------------------------------ */
  /* Output the SYMDEF header for these files			  */
  /* ------------------------------------------------------------ */
  for(cur=filelist; cur; cur=cur->next) {
    fprintf(output,"%s\n",cur->file);

    fputc('\t',output);
    for(name=cur->have; name; name=name->next) {
      strcpy(lowername,name->name);
      for(p=lowername; *p; p++) if (isupper(*p)) *p = tolower(*p);
      fprintf(output,"%s ",lowername);
    }
    fputc('*',output);
    fputc('\n',output);
  }

  /* ------------------------------------------------------------ */
  /* Dump the text of these files				  */
  /* ------------------------------------------------------------ */

  for(idx=1; idx<argc; idx++) {
    input = SafeOpen(argv[idx],"r");

    fprintf(output,"-%s\n",argv[idx]);
    while(fgets(linebuf,sizeof(linebuf),input)) {
      fputs(linebuf,output);
    }

    fclose(input);
  }

}

/**************************************************************************/
/* LOCAL  **************        SkipHeader         ************************/
/**************************************************************************/
/* PURPOSE:  Skip the SYMDEF information that proceeds the files in the	  */
/*	     archive.							  */
/**************************************************************************/
static void SkipHeader()
{
  int		c;

  for(;;) {
    c = getc(input);
    if ( c == '-' ) {
      ungetc(c,input);
      break;
    }

    while( c != EOF && c != '\n' ) c = getc(input);
    if ( c == EOF ) break;
  }
}
/**************************************************************************/
/* LOCAL  **************           InList          ************************/
/**************************************************************************/
/* PURPOSE:  Returns 1 if the name is in the list (or if the list is	  */
/*	     empty).							  */
/**************************************************************************/
static int InList(name,cnt,list)
     char	*name;
     int	cnt;
     char	*list[];
{
  int		i;

  if ( cnt <= 0 ) return 1;
  for(i=0;i<cnt;i++) {
    if ( strcmp(name,list[i]) == 0 ) return 1;
  }
  return 0;
}

/**************************************************************************/
/* LOCAL  **************          SkipToFile       ************************/
/**************************************************************************/
/* PURPOSE:  Reads lines into a buffer until the next file header is      */
/*	     found.  If the F parameter is set, the lines are echoed	  */
/*	     into the file.						  */
/**************************************************************************/
static void SkipToFile(linebuf,size,F)
     char   	*linebuf;
     unsigned	size;
     FILE	*F;
{
  while ( fgets(linebuf,size,input) ) {
    if ( linebuf[0] == '-' ) break;
    if ( F ) fputs(linebuf,F);
  }
}

/**************************************************************************/
/* LOCAL  **************       DisplayArchive      ************************/
/**************************************************************************/
/* PURPOSE:  Implements the ``p'' option.  Checks each file to see if it  */
/*	     is in the echo list.  If so, echo it to stdout.		  */
/**************************************************************************/
static void DisplayArchive(argc,argv)
     int	argc;
     char	*argv[];
{
  char		linebuf[256];
  char		filename[MAX_PATH];

  /* ------------------------------------------------------------ */
  /* Skip header block						  */
  /* ------------------------------------------------------------ */
  SkipHeader();

  /* ------------------------------------------------------------ */
  /* Process each file in turn					  */
  /* ------------------------------------------------------------ */
  while( !feof(input) ) {
    sscanf(&linebuf[1],"%s",filename);
    if ( InList(filename,argc-1,argv+1) ) {
      SkipToFile(linebuf,sizeof(linebuf),stdout);
    } else {
      SkipToFile(linebuf,sizeof(linebuf),(FILE*)NULL);
    }
  }
}

/**************************************************************************/
/* LOCAL  **************     ContentsOfArchive     ************************/
/**************************************************************************/
/* PURPOSE:  Implements the ``t'' option.  Checks each file to see if it  */
/*	     is in the select list.  If so, then its title is echoed.	  */
/**************************************************************************/
static void ContentsOfArchive(argc,argv)
     int	argc;
     char	*argv[];
{
  char		linebuf[256];
  char		filename[MAX_PATH];

  /* ------------------------------------------------------------ */
  /* Skip header block						  */
  /* ------------------------------------------------------------ */
  SkipHeader();

  /* ------------------------------------------------------------ */
  /* Process each file in turn					  */
  /* ------------------------------------------------------------ */
  while( !feof(input) ) {
    sscanf(&linebuf[1],"%s",filename);
    if ( InList(filename,argc-1,argv+1) ) puts(filename);
    SkipToFile(linebuf,sizeof(linebuf),(FILE*)NULL);
  }
}

/**************************************************************************/
/* LOCAL  **************     ExtractFromArchive    ************************/
/************************************************************************ **/
/* PURPOSE:  Implements the ``x'' option.  Checks each file to see if it  */
/*	     is in the select list.  If so, then it is dumped.		  */
/**************************************************************************/
static void ExtractFromArchive(argc,argv)
     int	argc;
     char	*argv[];
{
  char		linebuf[256];
  char		filename[MAX_PATH];
  FILE		*fp;

  /* ------------------------------------------------------------ */
  /* Skip header block						  */
  /* ------------------------------------------------------------ */
  SkipHeader();

  /* ------------------------------------------------------------ */
  /* Process each file in turn					  */
  /* ------------------------------------------------------------ */
  while( !feof(input) ) {
    sscanf(&linebuf[1],"%s",filename);
    if ( InList(filename,argc-1,argv+1) ) {
      if ( MakeTmpFile ) sprintf(filename,"#tmp%d.if1",MakeTmpFile);
      fp = SafeOpen(filename,"w");
      SkipToFile(linebuf,sizeof(linebuf),fp);
      fclose(fp);
    } else {
      SkipToFile(linebuf,sizeof(linebuf),(FILE*)NULL);
    }
  }
}

/**************************************************************************/
/* LOCAL  **************    OpenArchiveForInput    ************************/
/**************************************************************************/
/* PURPOSE:  Opens an archive file.  If the file is -, then it is left as */
/*	     stdin.							  */
/**************************************************************************/
static void OpenArchiveForInput(s)
     char	*s;
{
  if ( strcmp(s,"-") != 0 ) {
    fclose(input);
    input = fopen(s,"r");
    if ( !input ) {
      perror(s);
      exit(1);
    }
  }
}

/**************************************************************************/
/* GLOBAL **************         IF1ARCHIVER       ************************/
/**************************************************************************/
/* PURPOSE:  Creates and maintains IF1 archives.  The files are of the	  */
/*	     following form:  A SYMDEF header lists every filename	  */
/*	     in the archive followed by lines one listing the symbols	  */
/*	     that are defined in the file.  The list is terminated by a	  */
/*	     star. After the header, the files are appended with a one	  */
/*	     line header of the form -name.				  */
/**************************************************************************/
void main(argc,argv)
     int	argc;
     char	*argv[];
{
  Func	Operation = CreateArchive;
  char	*p;

  /* ------------------------------------------------------------ */
  /* Look for a temporary identifier as first argument		  */
  /* ------------------------------------------------------------ */
  if ( argv[1] && isdigit(argv[1][0]) ) {
    MakeTmpFile = atoi(argv[1]);
    argc--;
    argv++;
  }

  if ( argc < 3 )  exit(0);	/* No work */

  /* ------------------------------------------------------------ */
  /* Decode the option.  These basically follow the ar(1) unix	  */
  /* options.  Some are ignored, some are not implemented.	  */
  /* ------------------------------------------------------------ */
  for(p=argv[1];  *p; p++) {
    switch (*p) {
    case 'd':
    case 'm':
    case 'q':
    case 'r':
      fprintf(stderr,"Option %c not supported\n",*p);
      exit(1);

    case 'a':
    case 'b':
    case 'c':
    case 'i':
    case 'l':
    case 'o':
    case 'u':
    case 'v':
      /* Ignored */
      break;

    case 'p':
      OpenArchiveForInput(argv[2]);
      Operation = DisplayArchive;
      break;

    case 't':
      OpenArchiveForInput(argv[2]);
      Operation = ContentsOfArchive;
      break;

    case 'x':
      OpenArchiveForInput(argv[2]);
      Operation = ExtractFromArchive;
      break;

    default:
      fprintf(stderr,"Unknown option %c\n",*p);
      exit(1);
    }
  }

  Operation(argc-2,argv+2);

  exit(0);
}
