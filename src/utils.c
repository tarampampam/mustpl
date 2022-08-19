#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

char *substr(const char *src, int m, int n) {
	int len = n - m;
	char *dest = (char *) malloc(sizeof(char) * (len + 1));

	for (int i = m; i < n && (*(src + i) != '\0'); i++) {
		*dest = *(src + i);
		dest++;
	}

	*dest = '\0';

	return dest - len;
}

char *readFile(const char *filePath) {
	FILE *f = fopen(filePath, "rb");

	if (!f) {
		perror("Cannot open/read file");

		exit(1);
	}

	fseek(f, 0, SEEK_END);
	long fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *data = malloc(fileSize + 1);
	fread(data, fileSize, 1, f);
	fclose(f);

	data[fileSize] = '\0';

	return data;
}

void writeFile(const char *filePath, const char *text) {
	FILE *f = fopen(filePath, "w");

	if (!f) {
		perror("Cannot create file");

		exit(1);
	}

	fprintf(f, "%s", text);

	fclose(f);
}
