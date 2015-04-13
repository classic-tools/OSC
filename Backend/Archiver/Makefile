# **** FILE CREATION MASKS
SHELL = /bin/sh
DMASK = 700
XMASK = 755
MMASK = 744

.SUFFIXES: .c .o
	
CC           = cc -g
BIN_PATH     = ../../bin
MAN_PATH     = ../../Man
BINSTALL     = cp
PAR          =

OBJ    = archiver.o ifxstuff.o
SRC    = archiver.c ifxstuff.c
INCL   = world.h ../Library/IFX.h

MINIMAL=0
.c.o:
	${CC} -c $*.c
	-@if [ $(MINIMAL) = 1 ]; then /bin/rm -f $*.c; else cat /dev/null; fi

local: if1archive
optional: 
all: local optional

if1archive:${PAR} ${OBJ} ../Library/libifx.a
	${CC} -o if1archive ${OBJ} ../Library/libifx.a -lm

#------------------------------------------------------------
lint: lint.log

# 127 == main() returns random value to invocation environment
lint.log: $(SRC) $(COMMONSRC) $(INCL) ../Library/llib-lifx.ln
	@lint -u -v -woff 91,302,307,127 \
		`echo -DLINT $(CFLAGS) $(CC) | tr ' ' '\012' | egrep '^-D'` \
		$(SRC) $(COMMONSRC) ../Library/llib-lifx.ln -lm \
		| tee lint.log \
		| egrep -v '( )(free|malloc|strlen|lseek|read|isspace|isdigit)(.*)llib-lc'

#------------------------------------------------------------
install:
	/bin/rm -f ${BIN_PATH}/if1archive
	${BINSTALL} if1archive ${BIN_PATH}/if1archive
	chmod $(XMASK) ${BIN_PATH}/if1archive

clean:
	rm -f ${OBJ}
	rm -f if1archive
	rm -f lint.log

${OBJ}: ${INCL}

#
