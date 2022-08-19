#ifndef GUARD_CLI_H
#define GUARD_CLI_H

struct TemplateVariable {
		char *name, *value;
		struct TemplateVariable *next;
};

struct ExecProcess {
		char *path; // path to the file for execution
		char **args;
		size_t args_len;
		size_t _real_args_len; // only for internal usage
};

struct CliArguments {
		char *data;                    // (-d)--data
		char *data_file;               // (-f)--data-file
		struct TemplateVariable *vars; // (-v)--var
		char *output_file;             // (-o)--out
		char *template_file;           // <first arg>
		struct ExecProcess *exec;      // -- [command <...>]
};

void CliParseArguments(int argc, char **argv, struct CliArguments *args);

#endif //GUARD_CLI_H
