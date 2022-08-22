#!/usr/bin/make

.PHONY: test clean

test:
	Test_1=foo __Test_2="😎" ../../mustpl -f ./give.data.json ./give.template.txt 2> ./stderr.out 1> ./stdout.out; \
		exitCode=$$?; if [ $$exitCode -ne 0 ]; then echo "wrong exit code: $$exitCode"; exit 1; fi
	diff -u ./want.stderr.txt ./stderr.out
	diff -u ./want.stdout.txt ./stdout.out

clean:
	-rm -v ./*.out
