#ifndef GUARD_CLI_H
#define GUARD_CLI_H

struct ExecProcess {
		char *path; // path to the file for execution
		char **args;
		size_t args_len;
};

struct CliArguments {
		char *data;               // (-d)--data
		char *data_file;          // (-f)--data-file
		char *output_file;        // (-o)--out
		char *template_file;      // <first arg>
		struct ExecProcess *exec; // -- [command <...>]
};

void CliParseArguments(int argc, char **argv, struct CliArguments *args);

#endif //GUARD_CLI_H
