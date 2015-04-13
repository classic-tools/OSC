# **** FILE CREATION MASKS
SHELL=/bin/sh
DMASK = 700
XMASK = 755
MMASK = 744

CC           = cc
FF           = f77
BIN_PATH     = ../bin
MAN_PATH     = ../Man
BINSTALL     = cp
SINSTALL     = cp
OSC	     = osc
RANLIB       = touch
AR           = ar r
PAR          =

TestDirs     = Mathtest LivLoops Database

local: test
optional:
all: local optional

test:
	for d in $(TestDirs); do cd $$d; $(MAKE) OSC=$(OSC); cd ..; done

clean:
	for d in $(TestDirs); do cd $$d; $(MAKE) clean; cd ..; done

install:
	echo "Nothing to install"

lint:
	echo "Nothing to lint"

#
