
#include <argp.h>
#include <stdio.h>
#include <stropts.h>


#include <sys/socket.h>

#include "args.h"
#include "dumper.h"
#include "init.h"

static struct argp argp = {options, parse_opt, args_doc, doc};

static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{

	struct arguments *arguments = state->input;

	switch (key)
	{

	case 'p':
		arguments->promiscous = 1;
		break;

	case 'I':
		arguments->ipOnly = 1;
		break;
		
	case ARGP_KEY_ARG:
		if (state->arg_num > 1) {
			argp_usage(state);
		} else {
			arguments->args[state->arg_num] = arg;
		}
		break;
	case ARGP_KEY_END:
		if (state->arg_num < 2)
			argp_usage(state);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
	
}		
			


int main(int argc, char **argv)
{

	int soc;
	       
	struct arguments args;


	args.promiscous = 0;
	args.ipOnly = 0;

	argp_parse(&argp, argc, argv, 0, 0, &args);
	
	fprintf(stderr, "%s\n", args.args[0]);
	
	if ((soc = initSocket(args.args[0], args.ipOnly, args.promiscous)) < 0) {
		return -1;
	}

	
	return dump(soc, args.args[1]);
	

}
