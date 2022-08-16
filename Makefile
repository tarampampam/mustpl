#!/usr/bin/make

SHELL = /bin/sh
LDFLAGS = "-static"

.PHONY: all
all: ctpl

# This will output the help for each task. thanks to https://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
.PHONY: help
help: ## Show this help
	@printf "\033[33m%s:\033[0m\n" 'Available commands'
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "  \033[32m%-11s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

.PHONY: ctpl
ctpl: ## Compile ctpl
	$(CC) $(LDFLAGS) -o ./ctpl src/main.c
	strip ./ctpl
	ls -lh ./ctpl
	file ./ctpl
