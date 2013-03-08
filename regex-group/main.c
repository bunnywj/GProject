#include "stdinc.h"
#include "parser.h"
#include "nfa.h"
#include "dfa.h"
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REGEXNUM 10
#define GROUPNUM 3
#define FIRST_GE_NUM 1000
#define MAX(a,b) ((a)>(b)?(a):(b))

int VERBOSE = 0;
int DEBUG = 0;
unsigned long DFAdata[REGEXNUM+1][REGEXNUM+1]={0};

struct groupnode
{
	/* data */
	int group[REGEXNUM];
	int index[GROUPNUM-1];
	unsigned long totalDFA;
};

static void cal_all_DFA(FILE *, regex_parser *);
//static void regex_random_group_func(FILE *ruleset, regex_parser *parser, int n);

void random(int a[], int n);
bool init_group_func(FILE *ruleset, regex_parser *parser);
unsigned long cal_approximate_group_DFA(int group[], int num);
void init_first_generation(struct groupnode test_group[], int num);

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

	if(init_group_func(ruleset, parser)){
	
		cal_all_DFA(ruleset, parser);

		srand((unsigned)time(0));

		struct groupnode test_group[FIRST_GE_NUM];
		init_first_generation(test_group,FIRST_GE_NUM);
		
		// for(int i;i<FIRST_GE_NUM;i++)
		// 	printf("%u\n",test_group[i].totalDFA);

	}

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
static void cal_all_DFA(FILE *ruleset, regex_parser *parser){
	int num = parser->get_regex_num(ruleset);

	int group[num+1];
	int i;
	
	printf("test regex number is %u\n", num);

	// Test examples
	memset(group, 0, (num+1)*sizeof(int));	// clear the group

	for(i=1;i<=num;i++){
		group[i] = i;}	// index evaluation must start from group[1]
						// add the 3rd regular expression in the given ruleset
		//group[2] = 2;	// add the last regular expreesion in the given ruleset
		//group[3] = 3;	
	
	group[0] = num;	// the count of regular expressions is filled in group[0]
					// there are two regular expreesions in the group
	unsigned long size = parser->parse_regex_group(ruleset, group);
					// get the number of DFA states corresponding to the group
	printf("put all regex together, total number of DFAs is :%u\n\n", size);	
}

/* 随机打乱一个数组 */
void random(int a[], int n){
	int index, tmp, i;

	for (i = 0; i < n; i++)
	{
		index = rand() % (n - i) + i;
		if (index != i)
		{
			tmp = a[i];
			a[i] = a[index];
			a[index] = tmp;
		}
	}
}

/* init the data of 2-2 DFA */
bool init_group_func(FILE *ruleset, regex_parser *parser){
	int num = parser->get_regex_num(ruleset);
	
	//memset(DFAdata, 0, (num+1) * (num+1) *sizeof(unsigned long));	// clear

	if(num!=REGEXNUM){
		printf("The numbers of regexs don't match!\n");
		return 0;
	}

	int group[3];
	memset(group, 0, 3*sizeof(int));	// clear the group

	int i,j;

	group[0] = 1;
	for(i=1;i<=num;i++){
		group[1]=i;
		DFAdata[i][i] =  parser->parse_regex_group(ruleset, group);
	}

	group[0] = 2;
	for(i=1;i<=num;i++){
		group[1]=i;
		for(j=1;j<i;j++){
			group[2]=j;
			DFAdata[i][j] =  DFAdata[j][i] =  parser->parse_regex_group(ruleset, group);
		}
	}

	// for(i=0;i<=num;i++){
	// 	for(j=0;j<=num;j++){
	// 		printf("%u ",DFAdata[i][j]);
	// 	}
	// 	printf("\n");
	// }
	return 1;
}

/* cal the approximate sum of a group using adding 2-2 data*/
unsigned long cal_approximate_group_DFA(int group[], int num){
	int i,j;
	unsigned long totalDFA = 0;
	for(i=0;i<num;i++){
		for(j=i;j<num;j++){
			totalDFA += DFAdata[group[i]][group[j]];
		}
	}
	return totalDFA;
}

/* init the first feneration */
void init_first_generation(struct groupnode test_group[],int num){

	int i,j,k;
	int temp[REGEXNUM];
	int n1,n2,n3;
	int rand1,rand2;

	for(i=0;i<num;i++){

		for(k=0;k<REGEXNUM;k++)
			test_group[i].group[k] = k+1 ;

		random(test_group[i].group,REGEXNUM);

		// for(j=0;j<REGEXNUM;j++){
		// 	printf("%u ", test_group[i].group[j]);
		// }
		// printf("\n");

		rand1= rand() %(REGEXNUM-1) + 1;
		do{
			rand2= rand() %(REGEXNUM-1) + 1;
		}while(rand2 == rand1);
		test_group[i].index[1] = MAX(rand1,rand2);
		test_group[i].index[0] = rand1 + rand2 - test_group[i].index[1];

		memset(temp, 0, REGEXNUM * sizeof(int));
		memcpy(temp,test_group[i].group, test_group[i].index[0]*sizeof(int));
		n1 = cal_approximate_group_DFA(temp,test_group[i].index[0]);
		// for(k=0;k<REGEXNUM;k++){printf("%u ", temp[k]);}
		// printf("\n");

		memset(temp, 0, REGEXNUM * sizeof(int));
		memcpy(temp,test_group[i].group + test_group[i].index[0],(test_group[i].index[1]-test_group[i].index[0])*sizeof(int));
		n2 = cal_approximate_group_DFA(temp,test_group[i].index[1]-test_group[i].index[0]);
		// for(k=0;k<REGEXNUM;k++){printf("%u ", temp[k]);}
		// printf("\n");

		memset(temp, 0, REGEXNUM * sizeof(int));
		memcpy(temp,test_group[i].group+test_group[i].index[1],(REGEXNUM - test_group[i].index[1])*sizeof(int));
		n3 = cal_approximate_group_DFA(temp,REGEXNUM - test_group[i].index[1]);
		// for(k=0;k<REGEXNUM;k++){printf("%u ", temp[k]);}
		// printf("\n");

		test_group[i].totalDFA = n1+n2+n3;
		// printf("%u + %u + %u = %u\n\n",n1,n2,n3,test_group[i].totalDFA);
	}
}