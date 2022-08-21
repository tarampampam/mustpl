#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "envsubst.h"

struct buffer {
		char *data;
		unsigned int len, cap;
};

static struct buffer *newBuf(unsigned int cap) {
	struct buffer *b = malloc(sizeof(struct buffer));

	b->data = malloc(cap * sizeof(char));
	b->cap = cap;
	memset(b->data, '\0', b->cap);

	return b;
}

static void freeBuf(struct buffer *buf) {
	free(buf->data);
	free(buf);
}

static void emptyBuf(struct buffer *buf) {
	if (buf->len > 0) {
		memset(buf->data, '\0', buf->cap);
		buf->len = 0;
	}
}

static void writeInBuf(struct buffer *buf, const char c) {
	if (buf->cap <= buf->len + 1) {
		unsigned int newSize = buf->cap + 64; // growing size

		char *newAlloc = realloc(buf->data, newSize);

		if (newAlloc == NULL) {
			return;
		}

		buf->data = newAlloc;
		buf->cap = newSize;
		memset(buf->data + buf->len, '\0', buf->cap);
	}

	buf->data[buf->len++] = c;
}

static void writeStringInBuf(struct buffer *buf, const char *str) {
	for (unsigned int j = 0; str[j] != '\0'; j++) {
		writeInBuf(buf, str[j]);
	}
}

static void dumpBuf(struct buffer *buf) { // TODO: just for debug
	printf(
		"data pointer:\t%p\ndata len/cap:\t%d/%d\n",
		buf->data,
		buf->len,
		buf->cap
	);

	for (unsigned int i = 0; i < buf->cap; ++i) {
		printf("%d [%c] %d\t", i, buf->data[i], buf->data[i]);
	}

	printf("\n\n");
}

char *envsubst(const char *str) {
	unsigned int strLen = strlen(str);
	struct buffer *result = newBuf(strLen);
	struct buffer *envName = newBuf(32);
	struct buffer *envDef = newBuf(32);

	enum {
			DATA,
			ENV_NAME,
			ENV_DEFAULT,
	} state = DATA, prevState = DATA;

	for (unsigned int i = 0; i < strLen; i++) {
		// detect the state
		if (str[i] == '$' && str[i + 1] == '{') {
			i++; // jump forward
			prevState = state;
			state = ENV_NAME;

			continue;
		} else if (state == ENV_NAME && (str[i] == ':' && str[i + 1] == '-')) {
			i++; // jump forward
			prevState = state;
			state = ENV_DEFAULT;

			continue;
		} else if ((state == ENV_NAME || state == ENV_DEFAULT) && str[i] == '}') {
			i++;
			prevState = state;
			state = DATA;
		}

		const char c = str[i];

		// state processing
		switch (state) {
			case ENV_NAME:
				writeInBuf(envName, c);
				break;

			case ENV_DEFAULT:
				writeInBuf(envDef, c);
				break;

			case DATA:
				if (prevState == ENV_NAME || prevState == ENV_DEFAULT) {
					char *envVar = getenv(envName->data);

					if (envVar) {
						writeStringInBuf(result, envVar);
					} else if (envDef->len > 0) {
						writeStringInBuf(result, envDef->data);
					}

					emptyBuf(envName);
					emptyBuf(envDef);
				}

				writeInBuf(result, c);
		}
	}

	freeBuf(envName);
	freeBuf(envDef);

	char *data = result->data;
	free(result);

	return data;
}

int main() {
	char *input = "__$FOO ${bar} $FOO:def ${FOO:-def} bla-bla ${FOO2:-тест}";

	printf(
		"\ninput:\t%s\nwant:\t%s\nresult:\t%s\n",
		input,
		"__$FOO  $FOO:def yeah bla-bla тест",
		envsubst(input)
		);
}
