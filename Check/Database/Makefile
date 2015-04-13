SHELL=/bin/sh
OSC=osc

run: touch driver
	@-/bin/rm -f driver.test
	@driver > driver.test
	@-diff driver.test driver.answers >> test.log
	diff driver.test driver.answers
	@echo Compared OK

touch:

cdf.o: cdf.sis
	$(OSC) -module -noload -db database cdf.sis

driver: driver.sis cdf.o
	$(OSC) driver.sis cdf.o -db database -o driver

clean:
	@/bin/rm -f driver driver.o cdf.o database driver.test test.log

#


