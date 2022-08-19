#include <argp.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"
#include "cli.h"
#include "version.h"

// application version
const char *argp_program_version = APP_VERSION;

// bug-reporting address for the program
const char *argp_program_bug_address = "https://github.com/tarampampam/ctpl/issues";

// the exit status that argp will use when exiting due to a parsing error
error_t argp_err_exit_status = 2;

// options
static struct argp_option options[] = {
	{.doc ="Template data:", .flags = OPTION_DOC},
	{"data",      'd', "<json-string>", .doc = "Template data in JSON-string format (has higher priority than --data-file flag)"},
	{"data-file", 'f', "<file>", .doc = "Read template data from the JSON file"},

	{.doc = "Template variables:", .flags = OPTION_DOC},
	{"var",       'v', "foo:bar", .doc = "Override variable 'foo' in the template data with 'bar'. In addition, you can use '$FOO' as a value for reading values from the environment variables (use '$$' as a prefix to ignore this - '$$FOO' -> '$FOO')"},

	{.doc = "Output:", .flags = OPTION_DOC},
	{"out",       'o', "<out-file>", .doc = "Write output to the file instead of standard output"},

	{0},
};

static struct TemplateVariable *parseVariable(char *arg) {
	struct TemplateVariable *var = malloc(sizeof(struct TemplateVariable));

	unsigned argLen = strlen(arg);
	unsigned delimiterPosition = (strchr(arg, ':') - arg);

	if (delimiterPosition + 1 < argLen) {
		var->name = substr(arg, 0, (int) delimiterPosition);
		var->value = substr(arg, (int) delimiterPosition + 1, (int) argLen);

		unsigned valueLen = strlen(var->value);

		if (valueLen >= 2 && strstr(var->value, "$")) {
			char *newValue = substr(var->value, 1, (int) valueLen); // without `$` at the start

			if (strstr(var->value, "$$")) { // transform `$$FOO` to `$FOO`
				var->value = newValue; // drop one `$` ($$FOO -> $FOO)
			} else {
				char *envVar = getenv(newValue); // get the environment value for `FOO`

				if (envVar) {
					var->value = envVar;
				}
			}
		}
	}

	var->next = NULL;

	return var;
}

static error_t parseOptions(int key, char *arg, struct argp_state *state) {
	struct CliArguments *in = state->input;

	switch (key) {
		case 'd':
			in->data = arg;
			break;

		case 'f':
			in->data_file = arg;
			break;

		case 'o':
			in->output_file = arg;
			break;

		case 'v':
			if (!in->vars) { // lazy init
				in->vars = parseVariable(arg);
			} else {
				struct TemplateVariable *newVar = parseVariable(arg);
				struct TemplateVariable *current = in->vars;

				while (current->next) { // locate to the last element in the list
					current = in->vars->next;
				}

				current->next = newVar;
			}
			break;

		case ARGP_KEY_ARG:
			if (!in->template_file) {
				in->template_file = arg;
			} else {
				if (!in->exec) {
					struct ExecProcess *exec = malloc(sizeof(struct ExecProcess));
					exec->path = arg;
					exec->_real_args_len = state->argc;
					exec->args = malloc(exec->_real_args_len * sizeof(char *));

					in->exec = exec;
				} else {
					in->exec->args[in->exec->args_len] = arg;
					in->exec->args_len++;
				}
			}

			break;

		case ARGP_KEY_NO_ARGS:
			argp_error(state, "No template file provided");
			break;

		case ARGP_KEY_END: // make in validation
			if (in->data_file && access(in->data_file, F_OK) != 0) {
				argp_error(state, "The template data file (%s) doesn't exist", in->data_file);
			}

			if (!in->template_file) {
				argp_error(state, "No template file");
			}

			if (access(in->template_file, F_OK) != 0) {
				argp_error(state, "The template file (%s) doesn't exist", in->template_file);
			}

			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

void CliParseArguments(int argc, char **argv, struct CliArguments *args) {
	static struct argp argp = {
		options,
		parseOptions,
		"<template-file> [-- <exec-command>]",
		"\n{{ mustach }} templating tool. For more details about the templating engine and rules please follow "
		"this link: https://mustache.github.io/mustache.5.html",
		NULL,
		NULL,
		NULL
	};

	argp_parse(&argp, argc, argv, 0, 0, args);
}
