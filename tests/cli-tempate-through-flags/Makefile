#!/usr/bin/make

.PHONY: test clean

test:
	../../mustpl -f ./give.data.json -d '{"foo": "bar"}' -o ./tmp.out ./give.template.txt -- cat ./tmp.out 2> ./stderr.out 1> ./stdout.out; \
		exitCode=$$?; if [ $$exitCode -ne 0 ]; then tail -n +1 ./*.out; echo "wrong exit code: $$exitCode"; exit 1; fi
	diff -u ./want.stderr.txt ./stderr.out
	diff -u ./want.stdout.txt ./stdout.out

clean:
	-rm -v ./*.out
