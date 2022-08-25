#!/usr/bin/make

.PHONY: test clean

test:
	echo "blabla" > ./result.out
	../../mustpl -f ./give.data.json -o ./result.out ./give.template.txt -- sh -c "exit 123" 2> ./stderr.out 1> ./stdout.out; \
		exitCode=$$?; if [ $$exitCode -ne 123 ]; then tail -n +1 ./*.out; echo "wrong exit code: $$exitCode"; exit 1; fi
	diff -u ./want.file.txt ./result.out
	diff -u ./want.stderr.txt ./stderr.out
	diff -u ./want.stdout.txt ./stdout.out

clean:
	-rm -v ./*.out
