#ifndef _ARGS_H_
#define _ARGS_H_

static struct argp_option options[] = {
	{0, 'p', 0, OPTION_ARG_OPTIONAL, "Promiscous Mode"},
	{0, 'I', 0, OPTION_ARG_OPTIONAL, "IP Only"},
	{ 0 },
};

const char *argp_program_vesion = "";
const char *argp_program_bug_address =
	"contact@ytn86.net";
static char doc[] = "";
static char args_doc[] = "Interface File";

struct arguments
{
       	char *args[2];
	int promiscous;
	int ipOnly;
};

static error_t
parse_opt(int key, char *arg, struct argp_state *state);



#endif
