#include "stdinc.h"
#include "parser.h"
#include "nfa.h"
#include "dfa.h"
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int VERBOSE = 0;
int DEBUG = 0;

unsigned long **DFAdata;
int GROUPNUM = 4;

static void usage()
{
	fprintf(stderr,"\n");
	fprintf(stderr, "Usage: regex --parse|-p <regex_file> -n <group_number> [Options]\n"); 
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
		else if (strcmp(argv[i], "-n") == 0){
			i++;
			if(i==argc){
				fprintf(stderr,"Group number missing.\n");
				return 0;
			}
			GROUPNUM = atoi(argv[i]); 
			printf(">> to group into %d groups\n",GROUPNUM);
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


/* get a double random number between 0 and 1 */
double randd()
{
  return (double)rand()/RAND_MAX;
}

/* get a int random number between 1 and k */
int randi(int k)
{
  return rand() % k + 1; 
}

static void cal_total_DFA(FILE *ruleset, regex_parser *parser){
	int num = parser->get_regex_num(ruleset);

	int group[num+1];
	int i;
	
	printf("test regex number is %d\n", num);

	// Test examples
	memset(group, 0, (num+1)*sizeof(int));	// clear the group

	for(i=1;i<=num;i++){
		group[i] = i;}	// index evaluation must start from group[1]

	
	group[0] = num;	// the count of regular expressions is filled in group[0]
					// there are two regular expreesions in the group
	unsigned long size = parser->parse_regex_group(ruleset, group);
					// get the number of DFA states corresponding to the group
	printf("put all regex together, total number of DFAs is :%lu\n\n", size);	
}

/* init the data of 2-2 interact DFA */
void init_data(FILE *ruleset, regex_parser *parser){

	int num = parser->get_regex_num(ruleset);
	
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
				DFAdata[i][j] = DFAdata[j][i] = temp - DFAdata[i][i] - DFAdata[j][j];
		}
	}

	for(i=0;i<=num;i++){
		for(j=0;j<=num;j++){
			printf("%lu ",DFAdata[i][j]);
		}
		printf("\n");
	}
}

/* cal the approximate sum of DFA of a group using data of 2-2 matrix */
unsigned long cal_approximate_group_DFA(int group[], int num){
	int i,j;
	unsigned long fitness = 0;
	for(i=0;i<num;i++){
		for(j=i+1;j<num;j++){
			fitness += DFAdata[group[i]][group[j]];
		}
	}
	return fitness;
}

/* cal the approximate sum of DFA of a group using data of 2-2 matrix */
unsigned long cal_approximate_node_DFA(int index[], int REGEXNUM, int GROUPNUM){
	
	int i,j,k;
	int count[GROUPNUM+1];
	int group[GROUPNUM+1][REGEXNUM+1];
	unsigned long size = 0;
	
	memset(count, 0, (GROUPNUM+1)*sizeof(int));
	memset(group, 0, (GROUPNUM+1)*(REGEXNUM+1)*sizeof(int));

	for(k=1;k<=GROUPNUM;k++){

		for(j=0;j<REGEXNUM;j++){				
			if(index[j] == k){
				group[k][count[k]] = j + 1;				
				count[k] = count[k] + 1;
			}
		}
		size += cal_approximate_group_DFA(group[k],count[k]);
	}
	//printf("size = %lu\n", size);
	return size;
}

void GRELS(int REGEXNUM, int GROUPNUM){
	int i,j;
	int index[REGEXNUM];
	int tempindex[REGEXNUM];
	unsigned long min,temp;
	bool nochange;

	for(j=0;j<REGEXNUM;j++){
		index[j] =randi(GROUPNUM) ;
		printf("%d ", index[j]);
	}
	printf("\n");

	min = cal_approximate_node_DFA(index,REGEXNUM,GROUPNUM);
	memcpy(tempindex,index,REGEXNUM *sizeof(int));

	while(true){
		nochange = true;
		for(i=0;i<REGEXNUM;i++){
			for(j=1;j<=GROUPNUM;j++){
				index[i] = j;
				temp = cal_approximate_node_DFA(index,REGEXNUM,GROUPNUM);
				if(temp < min){
					min = temp;
					memcpy(tempindex,index,REGEXNUM *sizeof(int));
					nochange = false ;
					printf(">>min = %lu\n", min);
				}
			}
		}
		if((i>=REGEXNUM)&&nochange)
			break;
	}

	printf("min = %lu\n", min+DFAdata[0][0]);
	for(i=0;i<REGEXNUM;i++){
		printf("%d ", tempindex[i]);
	}
	printf("\n");

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

	srand((unsigned)time(0));

	//cal_total_DFA(ruleset, parser);

	init_data(ruleset, parser);

	int REGEXNUM = parser->get_regex_num(ruleset);

	GRELS(REGEXNUM,GROUPNUM);

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


