SHELL=/bin/sh
OSC=osc

run: touch basic basic.in basic.answers
	@basic basic.in > basic.test \
		-bformat "%c " \
		-cformat "'%c' " \
		-fformat "%.1f " \
		-dformat "%.1lf " \
		-iformat "%d "
	@-diff basic.test basic.answers >> test.log
	diff basic.test basic.answers
	@echo Compared OK

touch:

basic: basic.sis
	$(OSC) -noinline basic.sis -o basic -cpp > test.log 2>&1

clean:
	@/bin/rm -f basic basic.o basic.test test.log

#
