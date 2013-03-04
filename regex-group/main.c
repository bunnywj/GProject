#include "stdinc.h"
#include "parser.h"
#include "nfa.h"
#include "dfa.h"
#include <sys/time.h>

int VERBOSE = 0;
int DEBUG = 0;
static void regex_group_user_func(FILE *, regex_parser *);

static void usage()
{
	fprintf(stderr,"\n");
	fprintf(stderr, "Usage: regex --parse|-p <regex_file> [Options]\n"); 
	fprintf(stderr, "\nOptions:\n");
	fprintf(stderr, "       --debug|-d    enhanced verbosity level\n");
	fprintf(stderr, "\n");
	exit(0);
}

/* configuration */
static struct conf {
	char *regex_file;
	bool debug;
} config;

/* initialize the configuration */
void init_conf(){
	config.regex_file=NULL;
	config.debug=false;
}

/* print the configuration */
void print_conf(){
	fprintf(stderr,"\nCONFIGURATION: \n");
	if (config.regex_file) fprintf(stderr, "- RegEx file: %s\n",config.regex_file);
	if (config.debug) fprintf(stderr,"- debug mode\n");
}

/* parse the main call parameters */
static int parse_arguments(int argc, char **argv)
{
	int i=1;
	if (argc < 2) {
		usage();
		return 0;
	}
	while (i<argc){
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0){
			config.debug=1;
		}
		else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--parse") == 0){
			i++;
			if(i==argc){
				fprintf(stderr,"Regular expression file name missing.\n");
				return 0;
			}
			config.regex_file=argv[i];
		}
		else {
			fprintf(stderr,"Ignoring invalid option %s\n",argv[i]);
		}
		i++;
	}
	return 1;
}

/* check that the given file can be read/written */
void check_file(char *filename, char *mode){
	FILE *file=fopen(filename,mode);
	if (file==NULL){
		fprintf(stderr,"Unable to open file %s in %c mode",filename,mode);
		fatal("\n");
	}
	else {
		fclose(file);
	}
}

/*
 *  MAIN - entry point
 */
int main(int argc, char **argv){
	
	init_conf();

	while(!parse_arguments(argc,argv)) usage();
	printf(">> file: %s\n",config.regex_file);

	DEBUG=config.debug;
	
	if (config.regex_file==NULL){
		fatal("No data file - please use a regex file\n");
	}
	if (config.regex_file!=NULL){
		check_file(config.regex_file,"r");
	}

	NFA *nfa=NULL;
	DFA *dfa=NULL;
	
	FILE *ruleset=fopen(config.regex_file,"r");
	regex_parser *parser=new regex_parser(false,false);

	struct timeval start,end;
	gettimeofday(&start,NULL);
	/* BEGIN USER CODE */
	regex_group_user_func(ruleset, parser);
	/* END USER CODE */
	gettimeofday(&end,NULL);
	printf(">> preprocessing time: %ldms\n",
			(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000);

	if (parser!=NULL) delete parser;
	fclose(ruleset);
	/* Automata de-allocation */
	if (nfa!=NULL) delete nfa;
	if (dfa!=NULL) delete dfa;
		
	return 0;
}

/*
 * Please write your code HERE !!!
 *
 * You can call the function "unsigned long parse_regex_group(FILE *file, int group[])"
 * to get the number of DFA states corresponding to the given group of regular expressions.
 * The argument "file" is the file of regular expression ruleset, and the argument
 * "group" is the set of regular expression indexes.
 *
 * Note that:
 * group[0] is the number of regular expressions in the group array, and
 * group[1], group[2] ... group[num] are filled with the regular expression indexes.
 *
 * Before each evaluation, the group must be cleared first, and the evaluation
 * of the regular expreesion indexes must strictly start from the group[1].
 *
 */
static void regex_group_user_func(FILE *ruleset, regex_parser *parser){
	int num = parser->get_regex_num(ruleset);

	int group[num+1];
	
	// Test examples
	memset(group, 0, (num+1)*sizeof(int));	// clear the group
	group[1] = 1;	// index evaluation must start from group[1]
					// add the 3rd regular expression in the given ruleset
	group[2] = 2;	// add the last regular expreesion in the given ruleset
	group[3] = 3;	
	group[0] = 3;	// the count of regular expressions is filled in group[0]
					// there are two regular expreesions in the group
	unsigned long size = parser->parse_regex_group(ruleset, group);
					// get the number of DFA states corresponding to the group
	printf("test regex number is %u, group DFA size is %u\n", num, size);
	
}
