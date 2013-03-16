#include "stdinc.h"
#include "parser.h"
#include "nfa.h"
#include "dfa.h"
#include <sys/time.h>

int VERBOSE = 0;
int DEBUG = 0;
FILE *ruleset;
regex_parser *parser;

unsigned long **DFAdata;

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


/* *** */
void cal_interaction(int remain[], int num){

	//int num = parser->get_regex_num(ruleset);
	
	//memset(DFAdata, 0, (num+1) * (num+1) *sizeof(unsigned long));	// clear

	int i,j;
	unsigned long temp = 0;

	DFAdata = (unsigned long **) malloc(sizeof(unsigned long *)*(num+1));
	for(i=0 ; i < num+1 ; i++){
		DFAdata[i]=(unsigned long *) malloc(sizeof(unsigned long )*(num+1));
		memset(DFAdata[i], 0, (num+1) *sizeof(unsigned long));	// clear
	}

	int group[3];
	memset(group, 0, 3*sizeof(int));	// clear the group

	group[0] = 1;
	for(i=1;i<=num;i++){
		group[1]=i;
		DFAdata[i][i] =  parser->parse_regex_group(ruleset, group);
		DFAdata[0][0] += DFAdata[i][i];
	}

	group[0] = 2;
	for(i=2;i<=num;i++){
		group[1]=i;
		for(j=1;j<i;j++){
			group[2]=j;
			temp =  parser->parse_regex_group(ruleset, group) ;
			if(temp < DFAdata[i][i] + DFAdata[j][j])
				DFAdata[i][j] = DFAdata[j][i] =0;
			else
				DFAdata[i][j] = DFAdata[j][i] = 1;
		}
	}

	for(i=1;i<=num;i++){
		for(j=1;j<=num;j++){
			DFAdata[i][0] += DFAdata[i][j];
		}
		DFAdata[i][0] -= DFAdata[i][i];
		DFAdata[0][i] = DFAdata[i][0];
	}

	for(i=0;i<=num;i++){
		for(j=0;j<=num;j++){
			printf("%lu ",DFAdata[i][j]);
		}
		printf("\n");
	}
}

void init_data(){
	int num = parser->get_regex_num(ruleset);
	int group[num];

	int i;
	for(i=0;i<num;i++){
		group[i] = i;}

	cal_interaction(group,num);
}

/* init the data of 2-2 interact DFA */
void fang_yu(int num, unsigned long DFAlimit){
	//init_data();
	int i,j,k;
	int min,tempsum;
	unsigned long size;
	int *NG,*tempNG;
	NG = (int *) malloc(sizeof(int )*(num+1));
	tempNG = (int *) malloc(sizeof(int )*(num+1));
	memset(NG, 0, (num+1)*sizeof(int));
	memset(tempNG, 0, (num+1)*sizeof(int));

	bool istaken,isfirst;

	min = DFAdata[1][0];

	j=0;
	for(i=2;i<num;i++){
		if(DFAdata[i][0]<min){
			min=DFAdata[i][0];
			j=i;
		}
	}
	NG[0]++;
	NG[NG[0]]=j;

	memcpy(tempNG,NG,sizeof(int )*(num+1));

	for(int ii=1;ii<=10;ii++){

		min = 0;
		
		isfirst = true;
		for(i=1;i<=num;i++){
			istaken = false;
			
			tempsum = 0;
			for(j=1;j<=NG[0];j++){
				if(i == NG[j]) istaken =true;
			}

			if(!istaken){
				for(j=1;j<=NG[0];j++){
					tempsum += DFAdata[i][NG[j]];
				}

				if(isfirst){
					min = tempsum;
					k = i;
					isfirst = false;
				}
				else{
					if(tempsum < min){
						min = tempsum;
						k = i;
					}
				}			
			}
		}

		printf("k = %d\n", k);
		NG[0]++;
		NG[NG[0]]=k;

		for(j=1;j<=NG[0];j++){
			printf("%d ", NG[j]);
		}
		size = parser->parse_regex_group(ruleset, NG);
		printf("size = %lu\n\n", size);

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
	
	ruleset=fopen(config.regex_file,"r");
	parser=new regex_parser(false,false);

	struct timeval start,end;
	gettimeofday(&start,NULL);
	/* BEGIN USER CODE */

	//cal_total_DFA();

	init_data();

	int num = parser->get_regex_num(ruleset);

	fang_yu(num,100);

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

