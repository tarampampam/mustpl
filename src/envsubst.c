#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "envsubst.h"

struct buffer {
		char *data;
		unsigned int len, cap;
};

static struct buffer *newBuf(unsigned int cap) {
	struct buffer *b = malloc(sizeof(struct buffer));

	b->data = malloc(cap * sizeof(char));
	b->cap = cap;
	b->len = 0;
	memset(b->data, '\0', b->cap);

	return b;
}

static void emptyBuf(struct buffer *buf) {
	if (buf->len > 0) {
		memset(buf->data, '\0', buf->cap);
		buf->len = 0;
	}
}

static void writeInBuf(struct buffer *buf, const char c) {
	if (buf->cap <= buf->len + 1) {
		size_t newSize = buf->cap + 64; // growing size

		void *newAlloc = realloc(buf->data, newSize);

		if (newSize > buf->cap && newAlloc) {
			size_t diff = newSize - buf->cap;
			void *pStart = ((char *) newAlloc) + buf->cap;
			memset(pStart, '\0', diff);

			buf->data = newAlloc;
			buf->cap = newSize;
		}
	}

	buf->data[buf->len++] = c;
}

static void writeStringInBuf(struct buffer *buf, const char *str) {
	for (unsigned int j = 0; str[j] != '\0'; j++) {
		writeInBuf(buf, str[j]);
	}
}

/**
 * Parse the string and replace patterns in format `${ENV_NAME:-default_value}` with
 * the values from the environment (or default values after `:-` if provided).
 */
char *envsubst(const char *str) {
	size_t strLen = strlen(str);

	if (strLen < 4) {
		return (char *) str;
	}

	struct buffer *result = newBuf(strLen);
	struct buffer *envName = newBuf(32);
	struct buffer *envDef = newBuf(32);

	enum {
			DATA,
			ENV_NAME,
			ENV_DEFAULT,
	} state = DATA, prevState = DATA;
	bool flush = false;
	unsigned int nested = 0;
	unsigned int nestedInDef = 0;

	for (unsigned int i = 0; str[i] != '\0'; i++) {
		// detect the state
		if (str[i] == '$' && str[i + 1] == '{') {
			i++;
			nested++;
			prevState = state;
			state = ENV_NAME;

			continue;
		} else if ((str[i] == ':' && str[i + 1] == '-') && state == ENV_NAME) {
			i++;
			prevState = state;
			state = ENV_DEFAULT;

			continue;
		} else if (str[i] == '{' && nested > 0 && state == ENV_DEFAULT) {
			nestedInDef++;
		} else if (str[i] == '}' && nestedInDef == 0 && (state == ENV_NAME || state == ENV_DEFAULT)) {
			nested--;

			if (nested == 0) {
				i++;
				prevState = state;
				state = DATA;
				flush = true;
			}
		} else if (str[i] == '}' && nested > 0 && state == ENV_DEFAULT) {
			nestedInDef--;
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

				if (flush) {
					i--;
					flush = false;

					continue;
				}

				writeInBuf(result, c);
		}
	}

	free(envName->data);
	free(envName);

	free(envDef->data);
	free(envDef);

	char *data = result->data;
	free(result);

	return (char *) data;
}

//#include <assert.h>
//#include <stdio.h>
//
//// tests running: `gcc -o ./tmp/subs ./src/envsubst.c && ./tmp/subs`
//int main() {
//	putenv("Test_1=foo");
//	putenv("__#Test_2=😎");
//
//	assert(strcmp(
//		envsubst("__$_UNSET_VAR_ ${_UNSET_VAR_} ${_UNSET_VAR_:-default value 😎}"),
//		"__$_UNSET_VAR_  default value 😎"
//	) == 0);
//
//	assert(strcmp(
//		envsubst("${__#Test_2} ${__#Test_2:-foo}${_UNSET_VAR_:-def}${__#Test_2}"),
//		"😎 😎def😎"
//	) == 0);
//
//	assert(strcmp(
//		envsubst("${Test_1} ${Test_1:-def}${Test_1}"),
//		"foo foofoo"
//	) == 0);
//
//	assert(strcmp(
//		envsubst(
//			"__$FOO ${bar} $FOO:def ${Test_1:-def} ${Test_1} ${_UNSET_VAR_:-default} bla-bla ${FOO2:-тест}${ABC} ${}${}"),
//		 "__$FOO  $FOO:def foo foo default bla-bla тест "
//	) == 0);
//
//	assert(strcmp(
//		envsubst("${_UNSET_VAR_:-${Test_1}}"),
//		""
//	) == 0);
//
//	assert(strcmp(
//		envsubst("aaa ${}} ${${} bbb"),
//		"aaa } "
//	) == 0);
//
//	assert(strcmp(
//		envsubst("__${_UNSET_VAR_:-{\"string\"}}__"),
//		"__{\"string\"}__"
//	) == 0);
//
//	assert(strcmp(
//		envsubst("__${_UNSET_VAR_:- {} }__"),
//		"__ {} __"
//	) == 0);
//
//	assert(strcmp(
//		envsubst("{\"a\":1, \"root\": ${_UNSET_VAR_:-{\"foo\": 123}}, \"b\":2}"),
//		"{\"a\":1, \"root\": {\"foo\": 123}, \"b\":2}"
//	) == 0);
//
//	putenv("__JSON_VAR__={\"foo\": \"bar\"}");
//
//	printf("%s\n", envsubst("{\"a\":1, \"root\": ${__JSON_VAR__:-{}}, \"b\":2}")); // TODO: comment this line
//
//	assert(strcmp(
//		envsubst("{\"a\":1, \"root\": ${__JSON_VAR__:-{}}, \"b\":2}"),
//		"{\"a\":1, \"root\": {\"foo\": \"bar\"}, \"b\":2}"
//	) == 0);
//}
