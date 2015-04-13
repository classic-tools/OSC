SHELL=/bin/sh
XMASK = 555
MMASK = 444

.SUFFIXES: .c .f .o

CC           = cc
FF           = f77
BIN_PATH     = ../../bin
LIB_PATH     = ../../lib
INC_PATH     = ../../include
MAN_PATH     = ../../man
INSTALL      = cp
RANLIB       = touch
AR           = ar r
PAR          =
LIBM         = other

CONFIG_DIR   = ..

MINIMAL=0

SRCC  =	p-worker.c p-timer.c p-spawn.c              \
	p-wait.c p-ready.c p-ppp.c p-dsa.c          \
	p-error.c p-signal.c p-globals.c            \
	p-fibre.c p-util.c p-interface.c            \
	p-init.c p-pointer.c p-sdbx.c		    \
	p-tracer.c 				    \
	bcopy.c psignal.c vectorIO.c
OBJC  =	p-worker.o p-timer.o p-spawn.o              \
	p-wait.o p-ready.o p-ppp.o p-dsa.o          \
	p-error.o p-signal.o p-globals.o            \
	p-fibre.o p-util.o p-interface.o            \
	p-init.o p-pointer.o p-sdbx.o		    \
	p-tracer.o				    \
	bcopy.o psignal.o vectorIO.o

SRCF  =	mlib.f
OBJF  =	m$(LIBM).o

SRC0  =	p-srt0.c
OBJ0  =	p-srt0.o

CINC  = $(CONFIG_DIR)/config.h
SINC  =	arrays.h conf.h fibre.h interface.h locks.h process.h
OINC  =	options.h usage.h world.h
INCC  =	$(CINC) $(SINC) $(OINC)
INCF  =	sisalfli.inc

#CFLAGS=-DDEBUG_DSA -DVERBOSE
#CFLAGS=-DDEBUG_DSA

.c.o:
	${CC} -I$(CONFIG_DIR) $(CFLAGS) -c $*.c
	-@if [ $(MINIMAL) = 1 ]; then /bin/rm -f $*.c; else cat /dev/null; fi
.f.o:
	${FF} -c $*.f

local: libsisal.a srt0
optional:
all: local optional

libsisal.a: $(PAR) $(OBJC) $(OBJF)
	/bin/rm -f libsisal.a
	$(AR) libsisal.a $(OBJC) $(OBJF)

srt0: $(OBJ0)

${OBJC}: ${INCC}
${OBJ0}: ${INCC}
${OBJF}: ${INCF}

mcray.o:
	${FF} -c -Zp -Wd"-dc" -Wf"-astack" mlib.f
	mv mlib.o mcray.o
malliant.o:
	${FF} -Ogicv -AS -recursive -c -o malliant.o mlib.f
msgi.o:
	${FF} -c -O -o msgi.o mlib.f
mlinux.o:
	${FF} -c mlib.f
	mv mlib.o mlinux.o
mother.o:
	${FF} -c -o mother.o mlib.f
mnone.o:
	echo "static int mnone;" > mnone.c
	${CC} -c mnone.c

clean:
	rm -f ${OBJC}
	rm -f ${OBJF}
	rm -f ${OBJ0}
	rm -f libsisal.a
	rm -f lint.log

install:
	/bin/rm -f ${LIB_PATH}/p-srt0.o
	${INSTALL} p-srt0.o ${LIB_PATH}/p-srt0.o
	chmod $(MMASK) ${LIB_PATH}/p-srt0.o
	/bin/rm -f ${LIB_PATH}/libsisal.a
	${INSTALL} libsisal.a ${LIB_PATH}/libsisal.a
	${RANLIB}   ${LIB_PATH}/libsisal.a
	chmod $(MMASK) ${LIB_PATH}/libsisal.a

	/bin/rm -f ${INC_PATH}/sisalrt.h
	${INSTALL} world.h ${INC_PATH}/sisalrt.h
	chmod $(MMASK) ${INC_PATH}/sisalrt.h
	/bin/rm -f ${INC_PATH}/config.h
	${INSTALL} $(CONFIG_DIR)/config.h ${INC_PATH}/config.h
	chmod $(MMASK) ${INC_PATH}/config.h

	/bin/rm -f ${INC_PATH}/arrays.h
	${INSTALL} arrays.h ${INC_PATH}/arrays.h
	chmod $(MMASK) ${INC_PATH}/arrays.h
	/bin/rm -f ${INC_PATH}/conf.h
	${INSTALL} conf.h ${INC_PATH}/conf.h
	chmod $(MMASK) ${INC_PATH}/conf.h
	/bin/rm -f ${INC_PATH}/fibre.h
	${INSTALL} fibre.h ${INC_PATH}/fibre.h
	chmod $(MMASK) ${INC_PATH}/fibre.h
	/bin/rm -f ${INC_PATH}/interface.h
	${INSTALL} interface.h ${INC_PATH}/interface.h
	chmod $(MMASK) ${INC_PATH}/interface.h
	/bin/rm -f ${INC_PATH}/locks.h
	${INSTALL} locks.h ${INC_PATH}/locks.h
	chmod $(MMASK) ${INC_PATH}/locks.h
	/bin/rm -f ${INC_PATH}/process.h
	${INSTALL} process.h ${INC_PATH}/process.h
	chmod $(MMASK) ${INC_PATH}/process.h

	/bin/rm -f ${INC_PATH}/sisalfli.inc
	${INSTALL} sisalfli.inc ${INC_PATH}/sisalfli.inc
	chmod $(MMASK) ${INC_PATH}/sisalfli.inc

	/bin/rm -f ${MAN_PATH}/man1/s.out.1
	${INSTALL} s.out.1 ${MAN_PATH}/man1/s.out.1
	chmod $(MMASK) ${MAN_PATH}/man1/s.out.1

newoptions:
	$(BIN_PATH)/parseopts < options -usage "man.update" usage.h options.h
	awk '{print} /.\\" Start options/{exit}' < s.out.1 > s.out.1.new
	cat man.update >> s.out.1.new
	awk '/.\\" End options/{flag=1} {if (flag) print}' < s.out.1 >> s.out.1.new
	mv s.out.1 s.out.1.bu
	mv s.out.1.new s.out.1

# 127 == main() returns random value to invocation environment
# 183 == Previous declaration had prototype
lint: $(SRCC) $(SRC0)
	@lint -u -v -woff 91,302,307,127,183 \
		`echo -DLINT $(CFLAGS) $(CC) | tr ' ' '\012' | egrep ^-D` \
		$(SRCC) $(SRC0) -lm \
		| tee lint.log \
		| egrep -v '( )(bcopy|free|malloc|strlen|lseek|read|isalpha|strcmp|strncmp|isspace|isdigit)(.*)llib-lc'

#
