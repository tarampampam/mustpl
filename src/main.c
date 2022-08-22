#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../libs/cjson/cJSON.h"
#include "../libs/mustach/mustach-cjson.h"
#include "cli.h"
#include "envsubst.h"

static char *readFile(const char *filePath) {
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

static void writeFile(const char *filePath, const char *text) {
	FILE *f = fopen(filePath, "w");

	if (!f) {
		perror("Cannot create file");

		exit(1);
	}

	fprintf(f, "%s", text);

	fclose(f);
}

static const char *mustErrors[] = {
	"??? unreferenced ???",
	"system",
	"unexpected end",
	"empty tag",
	"tag too long",
	"bad separators",
	"too depth",
	"closing",
	"bad unescape tag",
	"invalid interface",
	"item not found",
	"partial not found",
	"undefined tag"
};

int main(int argc, char **argv) {
	struct CliArguments arguments = {NULL};

	CliParseArguments(argc, argv, &arguments);

	char *data, *template;

	if (arguments.data) { // data initialization
		data = arguments.data;
	} else if (arguments.data_file) {
		data = readFile(arguments.data_file);
	} else {
		data = "{}"; // fallback for the empty data and data_file
	}

	data = envsubst(data);

	template = readFile(arguments.template_file); // template initialization

	cJSON *jsonRoot = cJSON_Parse(data);

	if (!jsonRoot) {
		const char *error_ptr = cJSON_GetErrorPtr();

		if (error_ptr) {
			fprintf(stderr, "JSON parsing error: %s\n", error_ptr);
		} else {
			fprintf(stderr, "Unknown JSON parsing error\n");
		}

		return EXIT_FAILURE;
	}

	char *result;
	size_t *resultLength = 0;

	int s = mustach_cJSON_mem( // render the template
		template,
		0,
		jsonRoot,
		Mustach_With_AllExtensions,
		&result,
		resultLength
	);

	cJSON_Delete(jsonRoot);
	free(template);

	if (s != MUSTACH_OK) {
		s = -s;

		if (s < 1 || s >= (int)(sizeof mustErrors / sizeof * mustErrors)) {
			s = 0;
		}

		fprintf(stderr, "Template error: %s\n", mustErrors[s]);

		return EXIT_FAILURE;
	}

	if (arguments.output_file) {
		writeFile(arguments.output_file, result);
	} else {
		fprintf(stdout, "%s", result);
	}

	free(result);

	if (arguments.exec && arguments.exec->path) {
		char *app = arguments.exec->path;
		char **appArgv = malloc((arguments.exec->args_len + 2) * sizeof(char *));

		appArgv[0] = arguments.exec->path;

		for (size_t i = 0; i < arguments.exec->args_len; i++) {
			appArgv[i + 1] = arguments.exec->args[i];
		}

		appArgv[arguments.exec->args_len + 1] = NULL;

		errno = 0;

		if (execvp(app, appArgv) == -1) { // https://www.delftstack.com/howto/c/execvp-in-c/
			if (errno == EACCES) {
				fprintf(stdout, "Permission is denied");
			} else {
				perror("execvp failed");
			}

			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
