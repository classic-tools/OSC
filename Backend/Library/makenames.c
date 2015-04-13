#include <stdio.h>

char	*Compound[100];
char	*Graphs[100];
char	*Simple[900];
char	*Macros[900];
char	*Other[100];
int	 OtherOp[100];

#define NewCopy(s) ((char*)(strcpy((char*)(malloc(strlen(s)+1)),s)))

void
main()
{
  char		line[1024],name[100],Mac[100],*p;
  int		OpCode,MaxCompound = -1,MaxSimple = -1,MaxGraph = -1;
  int		OtherCount = 0,i,j;

  /* ------------------------------------------------------------ */
  /* Initialize tables						  */
  /* ------------------------------------------------------------ */
  for ( i = 0; i < sizeof(Simple)/sizeof(Simple[1]); i++ ) {
    Simple[i] = NULL;
  }
  for ( i = 0; i < sizeof(Macros)/sizeof(Macros[1]); i++ ) {
    Macros[i] = NULL;
  }
  for ( i = 0; i < sizeof(Compound)/sizeof(Compound[1]); i++ ) {
    Compound[i] = NULL;
  }
  for ( i = 0; i < sizeof(Graphs)/sizeof(Graphs[1]); i++ ) {
    Graphs[i] = NULL;
  }
  for ( i = 0; i < sizeof(Other)/sizeof(Other[1]); i++ ) {
    Other[i] = NULL;
  }
  for ( i = 0; i < sizeof(OtherOp)/sizeof(OtherOp[1]); i++ ) {
    OtherOp[i] = 0;
  }

  /* ------------------------------------------------------------ */
  /* Skip lines up to the start of definitions...		  */
  /* ------------------------------------------------------------ */
  while(gets(line)) {
    if ( strncmp(line,"/* START NODE DEFINITIONS",25) == 0 ) break;
  }

  /* ------------------------------------------------------------ */
  /* Now, decode each line in turn...				  */
  /* ------------------------------------------------------------ */
  while(gets(line)) {
    switch ( line[0] ) {
     case '/':
      if ( strncmp(line,"/* END NODE DEFINITIONS",23) == 0 ) goto Done;
      break;

     case '#':
      if ( strncmp(line,"#define",7) == 0 ) {
	sscanf(line+7,"%s %d",name,&OpCode);

	if ( OpCode < 100 ) {
	  if ( OpCode > MaxCompound ) MaxCompound = OpCode;
	  Compound[OpCode] = NewCopy(name);

	} else if ( OpCode < 1000 ) {
	  if ( OpCode > MaxSimple ) MaxSimple = OpCode;
	  Simple[OpCode-100] = NewCopy(name);
	  /* If this has a defined macro (a = name in a comment), use */
	  /* it as the macro name.  Otherwise, use the IF name */
	  for(p=line; *p; p++) {
	    if ( *p == '/' && *(p+1) == '*' ) break;
	  }
	  if ( *p ) p += 2;	/* Skip the comment */
	  for(;*p && *p != '='; p++); /* Skip to the equal */
	  if ( *p ) {
	    sscanf(p+1,"%s",Mac);
	  } else {
	    strcpy(Mac,name+2);
	  }
	  Macros[OpCode-100] = NewCopy(Mac);

	} else if ( OpCode < 2000 ) {
	  if ( OpCode > MaxGraph ) MaxGraph = OpCode;
	  Graphs[OpCode-1000] = NewCopy(name);

	} else {
	  Other[OtherCount]	= NewCopy(name);
	  OtherOp[OtherCount]	= OpCode;
	  OtherCount++;
	}
      }
      break;
   
     default:
      ;
    }
  }

 Done:
  puts("/* This code is mechanically produced.  Make changes in IFX.h */");
  puts("/* and ``make nametable.c'' to reproduce. */\n");
  puts("#include \"world.h\"");
  puts("static char Undefined[] = \"***NoDefinition\";\n");

  /* ------------------------------------------------------------ */
  printf("static char\t*cnames[%d] = {\n",MaxCompound+1);
  for(i=0;i<=MaxCompound;i++) {
    printf("/* %4d */ %c%s%c%c\n",
	   i,
	   (Compound[i])?('"'):(' '),
	   (Compound[i])?(Compound[i]+2):("Undefined"),
	   (Compound[i])?('"'):(' '),
	   (i==MaxCompound)?(' '):(',')
	   );
  }
  puts("};\n");

  /* ------------------------------------------------------------ */
  printf("static char\t*snames[%d] = {\n",MaxSimple-100+1);
  for(i=100;i<=MaxSimple;i++) {
    printf("/* %4d */ %c%s%c%c\n",
	   i,
	   (Simple[i-100])?('"'):(' '),
	   (Simple[i-100])?(Simple[i-100]+2):("Undefined"),
	   (Simple[i-100])?('"'):(' '),
	   (i==MaxSimple)?(' '):(',')
	   );
  }
  puts("};\n");

  /* ------------------------------------------------------------ */
  printf("static char\t*macros[%d] = {\n",MaxSimple-100+1);
  for(i=100;i<=MaxSimple;i++) {
    printf("/* %4d */ %c%s%c%c\n",
	   i,
	   (Macros[i-100])?('"'):(' '),
	   (Macros[i-100])?(Macros[i-100]):("Undefined"),
	   (Macros[i-100])?('"'):(' '),
	   (i==MaxSimple)?(' '):(',')
	   );
  }
  puts("};\n");

  /* ------------------------------------------------------------ */
  printf("static char\t*gnames[%d] = {\n",MaxGraph-1000+1);
  for(i=1000;i<=MaxGraph;i++) {
    printf("/* %4d */ %c%s%c%c\n",
	   i,
	   (Graphs[i-1000])?('"'):(' '),
	   (Graphs[i-1000])?(Graphs[i-1000]+2):("Undefined"),
	   (Graphs[i-1000])?('"'):(' '),
	   (i==MaxGraph)?(' '):(',')
	   );
  }
  puts("};\n");

  /* ------------------------------------------------------------ */
  /* ------------------------------------------------------------ */
  /* ------------------------------------------------------------ */
  for(j=0; j<2; j++) {
    if ( j == 0 ) {
      puts(  "char *GetNodeName(n)");
      puts(  "  PNODE n;");
      puts(  "{");
      puts(  "  int opcode = n->type;");
    } else {
      puts(  "char *OpCodeNameOf(opcode)");
      puts(  "  int opcode;");
      puts(  "{");
    }
    printf("  if ( opcode >= 0 && opcode <= %d ) return cnames[opcode];\n",
	   MaxCompound);
    printf("  if ( opcode >= 100 && opcode <= %d ) return snames[opcode-100];\n",
	   MaxSimple);
    printf("  if ( opcode >= 1000 && opcode <= %d ) return gnames[opcode-1000];\n",
	   MaxGraph);
    for(i=0; i<OtherCount; i++) {
      printf("  if ( opcode == %d ) return \"%s\";\n",OtherOp[i],Other[i]+2);
    }


    puts(  "  return Undefined;");
    puts(  "}");
  }

  puts("char *GetMacro(n)");
  puts("PNODE n;");
  puts("{");
  puts(  "  int opcode = n->type;");
  printf("  if ( opcode >= 100 && opcode <= %d ) return macros[opcode-100];\n",
	 MaxSimple);
  puts("  FPRINTF( stderr, \"HELP: n->type = %d n->if1line %d\\n\", n->type, n->if1line );");
  puts("  Error1( \"GetMacro: REQUEST FOR GRAPH OR COMPOUND MACRO NAME\" );");
  puts("  return NULL;");
  puts("}\n");


  puts("char *GraphName(i) int i; { return gnames[i]; }\n");
  puts("char *SimpleName(i) int i; { return snames[i]; }\n");
  puts("char *CompoundName(i) int i; { return cnames[i]; }\n");
  puts("char *AtName(i) int i; { return snames[IFAAddLAT+i-IFAAddH]; }\n");
  exit(0);
}

/* $Log: makenames.c,v $
 * Revision 1.3  1993/06/02  23:46:04  miller
 * A cast of strcpy (in the NewCopy macro) was causing problems for
 * GCC.
 *
 * Revision 1.2  1993/02/24  18:34:49  miller
 * Fixed a problem with the generations of AtNode names.
 *
 * Revision 1.1  1993/01/21  23:30:56  miller
 * Initial version of the IFX library.  It replaces the if[12]build.c
 * read.c timer.c util.c and write.c and if[12].h files from the
 * backend phases.
 * */
