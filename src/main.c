#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../libs/cjson/cJSON.h"
#include "../libs/mustach/mustach-cjson.h"
#include "utils.h"
#include "cli.h"

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

	template = readFile(arguments.template_file); // template initialization

//	// TODO implement variables support
//	struct TemplateVariable *ptr = arguments.vars;
//	while (ptr) {
//		printf("==var== (%s,%s) \n", ptr->name, ptr->value);
//		ptr = ptr->next;
//	}

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

	mustach_cJSON_mem( // render the template
		template,
		0,
		jsonRoot,
		Mustach_With_AllExtensions,
		&result,
		resultLength
	);

	cJSON_Delete(jsonRoot);
	free(template);

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
