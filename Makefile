#!/usr/bin/make

version      ?= 0.0.0-undefined
CC           ?= gcc
TARGET        = mustpl
LDFLAGS       = -std=gnu99 -s -static -Wall -Werror -Wl,--build-id=none
CFLAGS        = -std=gnu99 -c -fPIC -Wall -Wextra -g -Os
LIBARGP_FILES = argp-ba argp-eexst argp-fmtstream argp-help argp-parse argp-pv argp-pvh
LIBARGP_SRC   = $(foreach file,$(LIBARGP_FILES),libs/argp/$(file).c)
LIBARGP_OBJ   = $(foreach file,$(LIBARGP_FILES),obj/$(file).o)
MUSTACH_FILES = mustach mustach-wrap mustach-cjson
MUSTACH_SRC   = $(foreach file,$(MUSTACH_FILES),libs/mustach/$(file).c)
MUSTACH_OBJ   = $(foreach file,$(MUSTACH_FILES),obj/$(file).o)

.PHONY: src/version.h $(TARGET) clean

$(TARGET): obj/libargp.a obj/libcjson.a obj/libmustach.a obj/envsubst.o obj/cli.o obj/main.o
	$(CC) $(LDFLAGS) -L./obj $(wildcard obj/*.o) -o $(TARGET) -largp -lcjson -lmustach
	@-file $(TARGET) # print file info

obj/libargp.a: $(LIBARGP_SRC)
	$(foreach file,$(LIBARGP_FILES), $(CC) $(CFLAGS) -Wno-unused-parameter -o obj/$(file).o libs/argp/$(file).c;)
	ar cr obj/libargp.a $(LIBARGP_OBJ) && rm $(LIBARGP_OBJ)

obj/libcjson.a: libs/cjson/cJSON.c
	$(CC) $(CFLAGS) -o obj/cjson.o libs/cjson/cJSON.c
	ar cr obj/libcjson.a obj/cjson.o && rm obj/cjson.o

obj/libmustach.a: $(MUSTACH_SRC)
	$(foreach file,$(MUSTACH_FILES), $(CC) $(CFLAGS) -o obj/$(file).o libs/mustach/$(file).c;)
	ar cr obj/libmustach.a $(MUSTACH_OBJ) && rm $(MUSTACH_OBJ)

obj/envsubst.o: src/envsubst.c
	$(CC) $(CFLAGS) -o $@ $<

obj/cli.o: src/cli.c src/version.h
	$(CC) $(CFLAGS) -o $@ $<

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -o $@ $<

src/version.h:
	@printf '// Code generated by `make version.h`; DO NOT EDIT.\n\n#define APP_VERSION "%s"\n' "$(version)" > $@

test: ## Run tests
	@set -e; for n in $(wildcard tests/*); do \
		$(MAKE) -C $$n test clean; \
	done

clean: ## Cleaning
	-rm $(TARGET) src/version.h obj/*.o obj/*.a
