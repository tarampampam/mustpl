#!/usr/bin/make

.PHONY: test clean

test:
	../../mustpl 2> ./stderr.out 1> ./stdout.out; \
		exitCode=$$?; if [ $$exitCode -ne 1 ]; then echo "wrong exit code: $$exitCode"; exit 1; fi
	diff -u ./want.stderr.txt ./stderr.out
	diff -u ./want.stdout.txt ./stdout.out

clean:
	-rm -v ./*.out
