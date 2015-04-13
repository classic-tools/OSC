# **** FILE CREATION MASKS
SHELL=/bin/sh
DMASK = 700
XMASK = 755
MMASK = 744

.SUFFIXES: .c .o

CC           = cc
BIN_PATH     = ../../bin
MAN_PATH     = ../../Man
BINSTALL     = cp
SINSTALL     = cp
PAR          =

OBJLD  = p2clib.o file1.o file2.o file3.o
INCL   = p2c.h frontend.h

MINIMAL=0

.c.o:
	${CC} -c $*.c
	-@if [ $(MINIMAL) = 1 ]; then /bin/rm -f $*.c; else cat /dev/null; fi

local: sisal
optional:
all: local optional

sisal:${PAR} ${OBJLD}
	${CC} -o sisal ${OBJLD}

install:
	/bin/rm -f ${BIN_PATH}/sisal
	${BINSTALL} sisal ${BIN_PATH}/sisal
	chmod $(XMASK) ${BIN_PATH}/sisal
	/bin/rm -f ${BIN_PATH}/etablebin
	${SINSTALL} etablebin ${BIN_PATH}/etablebin
	chmod $(MMASK) ${BIN_PATH}/etablebin
	/bin/rm -f ${BIN_PATH}/ptablebin
	${SINSTALL} ptablebin ${BIN_PATH}/ptablebin
	chmod $(MMASK) ${BIN_PATH}/ptablebin

clean:
	rm -f ${OBJLD}
	rm -f sisal

${OBJLD}:   ${INCL}
