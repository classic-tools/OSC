# **** FILE CREATION MASKS
SHELL=/bin/sh
DMASK = 700
XMASK = 755
MMASK = 744

CC           = cc -g -DMAX_PROCS=1
BIN_PATH     = ../bin
MAN_PATH     = ../Man
BINSTALL     = cp
SINSTALL     = cp
BP           = \"${BIN_PATH}\"


SRC  = osc.c
PSRC = spp2.c

OOBJ = osc.o
SOBJ = spp2.o

MINIMAL=0

local: osc spp2
optional:
all: local optional

clean:
	rm -f ${OOBJ} $(SOBJ)
	rm -f osc spp2
	rm -f man.update osc.m.bu osc.m.new
	rm -f lint.log

newoptions:
	../_PARSEOPTS/parseopts < options -usage "man.update" usage.h options.h
	awk -e '{print} /.\\" Start options/{exit}' < osc.m > osc.m.new
	cat man.update >> osc.m.new
	awk -e '/.\\" End options/{flag=1} {if (flag) print}' < osc.m >> osc.m.new
	mv osc.m osc.m.bu
	mv osc.m.new osc.m

osc: $(OOBJ)
	${CC} -o osc $(OOBJ)

osc.o:  osc.c usage.h options.h ../Makefile
	$(CC) -c osc.c -DBIN_PATH=${BP} 
	-@if [ $(MINIMAL) = 1 ]; then /bin/rm -f $*.c; else cat /dev/null; fi

spp2: $(SOBJ)
	${CC} -o spp2 $(SOBJ)

spp2.o: spp2.c ../Makefile
	$(CC) -c spp2.c
	-@if [ $(MINIMAL) = 1 ]; then /bin/rm -f $*.c; else cat /dev/null; fi

install:
	/bin/rm -f ${BIN_PATH}/osc
	${BINSTALL} osc ${BIN_PATH}/osc
	chmod $(XMASK) ${BIN_PATH}/osc
	/bin/rm -f ${BIN_PATH}/spp2
	${BINSTALL} spp2 ${BIN_PATH}/spp2
	chmod $(XMASK) ${BIN_PATH}/spp2
	/bin/rm -f ${BIN_PATH}/speedups
	${BINSTALL} speedups ${BIN_PATH}/speedups
	chmod $(XMASK) ${BIN_PATH}/speedups
	/bin/rm -f ${BIN_PATH}/xspeedups
	${BINSTALL} xspeedups ${BIN_PATH}/xspeedups
	chmod $(XMASK) ${BIN_PATH}/xspeedups
	/bin/rm -f ${BIN_PATH}/spprun
	${BINSTALL} spprun ${BIN_PATH}/spprun
	chmod $(XMASK) ${BIN_PATH}/spprun
	/bin/rm -f ${MAN_PATH}/osc.l
	${SINSTALL} osc.m ${MAN_PATH}/osc.l
	chmod $(XMASK) ${MAN_PATH}/osc.l
	${SINSTALL} speedups.m ${MAN_PATH}/speedups.l
	chmod $(XMASK) ${MAN_PATH}/speedups.l
	${SINSTALL} xspeedups.m ${MAN_PATH}/xspeedups.l
	chmod $(XMASK) ${MAN_PATH}/xspeedups.l

# 127 == main() returns random value to invocation environment
# 183 == Previous declaration had prototype
lint: $(SRC) $(PSRC)
	@lint -u -v -woff 91,302,307,127,183 \
		`echo -DLINT $(CFLAGS) -DBIN_PATH=${BP} $(CC) | tr ' ' '\012' | egrep ^-D | egrep -v "^-DUSE(CC|FF)="`\
		$(SRC) -lm \
		| tee lint.log \
		| egrep -v '( )(bcopy|free|malloc|strlen|lseek|read|isalpha|strcmp|strncmp|strcpy|strncpy|strcat|isspace|isdigit)(.*)llib-lc'

#
