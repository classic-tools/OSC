SHELL=/bin/sh
OSC=osc

run: runloop1 runloop2 runloop3

runloop1: touch loop1 input.1 loop1.answers
	@loop1 input.1 > loop1.test \
		-bformat "%c " \
		-cformat "'%c' " \
		-fformat "%.4f " \
		-dformat "%.4lf " \
		-iformat "%d "
	@-diff loop1.test loop1.answers >> test.log
	diff loop1.test loop1.answers
	@echo Loop1 Compared OK

runloop2: touch loop2 input.2 loop2.answers
	@loop2 input.2 > loop2.test \
		-bformat "%c " \
		-cformat "'%c' " \
		-fformat "%.4f " \
		-dformat "%.4lf " \
		-iformat "%d "
	@-diff loop2.test loop2.answers >> test.log
	diff loop2.test loop2.answers
	@echo Loop2 Compared OK

runloop3: touch loop3 input.3 loop3.answers
	@loop3 input.3 > loop3.test -dformat "%.6lf"
	@-diff loop3.test loop3.answers >> test.log
	diff loop3.test loop3.answers
	@echo Loop3 Compared OK

touch:

loop1: loop1.sis
	$(OSC) loop1.sis -o loop1 > test.log 2>&1
loop2: loop2.sis
	$(OSC) loop2.sis -o loop2 >> test.log 2>&1
loop3: loop3.sis
	$(OSC) loop3.sis -o loop3 >> test.log 2>&1

clean:
	@/bin/rm -f test.log loop1 loop1.o loop1.test \
			    loop2 loop2.o loop2.test \
			    loop3 loop3.o loop3.test

#