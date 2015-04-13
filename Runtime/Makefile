# **** FILE CREATION MASKS
SHELL=/bin/sh
DMASK = 700
XMASK = 755
MMASK = 744

.SUFFIXES: .c .o

CC           = cc
FF           = f77
BIN_PATH     = ../bin
MAN_PATH     = ../Man
BINSTALL     = cp
SINSTALL     = cp
RANLIB       = touch
AR           = ar r
PAR          =

MINIMAL=0

SRC  =	p-worker.c p-timer.c p-spawn.c              \
	p-wait.c p-ready.c p-ppp.c p-dsa.c          \
	p-error.c p-signal.c p-globals.c            \
	p-fibre.c p-util.c p-interface.c            \
	p-init.c p-pointer.c p-sdbx.c		    \
	p-tracer.c p-srt0.c			    \
	bcopy.c psignal.c vectorIO.c

PMF  =	p-worker.o p-timer.o p-spawn.o              \
	p-wait.o p-ready.o p-ppp.o p-dsa.o          \
	p-error.o p-signal.o p-globals.o            \
	p-fibre.o p-util.o p-interface.o            \
	p-init.o p-pointer.o p-sdbx.o		    \
	p-tracer.o				    \
	bcopy.o psignal.o vectorIO.o

SRT0 =	p-srt0.o

INCL =	world.h conf.h locks.h fibre.h process.h arrays.h interface.h

.c.o:
	${CC} -c $*.c
	-@if [ $(MINIMAL) = 1 ]; then /bin/rm -f $*.c; else cat /dev/null; fi

#${CC} -DDSA_DEBUG -c $*.c

local: libsisal.a srt0
optional:
all: local optional

libsisal.a:${PAR} ${PMF} srt0
	/bin/rm -f libsisal.a
	${AR} libsisal.a ${PMF}

srt0: ${SRT0}

p-init.o: p-init.c options.h usage.h

clean:
	rm -f ${PMF}
	rm -f ${SRT0}
	rm -f libsisal.a
	rm -f mlib.o
	rm -f lint.log

cray:
	${FF} -c -Zp -Wd"-dc" -Wf"-astack" mlib.f
	${AR} libsisal.a mlib.o

alliant:
	${FF} -Ogicv -AS -recursive -c mlib.f
	${AR} libsisal.a mlib.o

sgi:
	${FF} -c -O mlib.f
	${AR} libsisal.a mlib.o

other:
	${FF} -c mlib.f
	${AR} libsisal.a mlib.o

install:
	/bin/rm -f ${BIN_PATH}/p-srt0.o
	${SINSTALL} p-srt0.o ${BIN_PATH}/p-srt0.o
	chmod $(MMASK) ${BIN_PATH}/p-srt0.o
	/bin/rm -f ${BIN_PATH}/libsisal.a
	${SINSTALL} libsisal.a ${BIN_PATH}/libsisal.a
	${RANLIB}   ${BIN_PATH}/libsisal.a
	chmod $(MMASK) ${BIN_PATH}/libsisal.a
	/bin/rm -f ${BIN_PATH}/conf.h
	${SINSTALL} conf.h ${BIN_PATH}/conf.h
	chmod $(MMASK) ${BIN_PATH}/conf.h
	/bin/rm -f ${BIN_PATH}/locks.h
	${SINSTALL} locks.h ${BIN_PATH}/locks.h
	chmod $(MMASK) ${BIN_PATH}/locks.h
	/bin/rm -f ${BIN_PATH}/process.h
	${SINSTALL} process.h ${BIN_PATH}/process.h
	chmod $(MMASK) ${BIN_PATH}/process.h
	/bin/rm -f ${BIN_PATH}/fibre.h
	${SINSTALL} fibre.h ${BIN_PATH}/fibre.h
	chmod $(MMASK) ${BIN_PATH}/fibre.h
	/bin/rm -f ${BIN_PATH}/arrays.h
	${SINSTALL} arrays.h ${BIN_PATH}/arrays.h
	chmod $(MMASK) ${BIN_PATH}/arrays.h
	/bin/rm -f ${BIN_PATH}/interface.h
	${SINSTALL} interface.h ${BIN_PATH}/interface.h
	chmod $(MMASK) ${BIN_PATH}/interface.h
	/bin/rm -f ${BIN_PATH}/sisalrt.h
	${SINSTALL} world.h ${BIN_PATH}/sisalrt.h
	chmod $(MMASK) ${BIN_PATH}/sisalrt.h
	/bin/rm -f ${MAN_PATH}/s.out.l
	${SINSTALL} s.out.m ${MAN_PATH}/s.out.l
	chmod $(MMASK) ${MAN_PATH}/s.out.l

${PMF}:   ${INCL}
${SRT0}:  ${INCL}

newoptions:
	../_PARSEOPTS/parseopts < options -usage "man.update" usage.h options.h
	awk '{print} /.\\" Start options/{exit}' < s.out.m > s.out.m.new
	cat man.update >> s.out.m.new
	awk '/.\\" End options/{flag=1} {if (flag) print}' < s.out.m >> s.out.m.new
	mv s.out.m s.out.m.bu
	mv s.out.m.new s.out.m

# 127 == main() returns random value to invocation environment
# 183 == Previous declaration had prototype
lint: $(SRC)
	@lint -u -v -woff 91,302,307,127,183 \
		`echo -DLINT $(CFLAGS) $(CC) | tr ' ' '\012' | egrep ^-D` \
		$(SRC) -lm \
		| tee lint.log \
		| egrep -v '( )(bcopy|free|malloc|strlen|lseek|read|isalpha|strcmp|strncmp|isspace|isdigit)(.*)llib-lc'

#
