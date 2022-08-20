#!/usr/bin/make

.PHONY: test clean

test:
	../../mustpl -d '{"foo": "baz123 321"}' -o ./result.out ./give.template.txt ./script.sh 2> ./stderr.out 1> ./stdout.out; \
		exitCode=$$?; if [ $$exitCode -ne 33 ]; then tail -n +1 ./*.out; echo "wrong exit code: $$exitCode"; exit 1; fi
	diff -u ./want.file.txt ./result.out
	diff -u ./want.stderr.txt ./stderr.out
	diff -u ./want.stdout.txt ./stdout.out

clean:
	-rm -v ./*.out
