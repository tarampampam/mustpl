#!/usr/bin/make

.PHONY: test clean

.ONESHELL:
test:
	__JSON_LIST__='{"foo": "bar", "baz": "blah"}' ../../mustpl -f ./give.data.json ./give.template.txt 2> ./stderr.out 1> ./stdout.out
	diff -u ./want.stderr.txt ./stderr.out || exit 1
	diff -u ./want.stdout.txt ./stdout.out || exit 1

	../../mustpl -f ./give.data.json ./give.template.txt 2> ./stderr.out 1> ./stdout.out
	diff -u ./want-fallback.stderr.txt ./stderr.out || exit 1
	diff -u ./want-fallback.stdout.txt ./stdout.out || exit 1

clean:
	-rm -v ./*.out
