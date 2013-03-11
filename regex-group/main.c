#include "stdinc.h"
#include "parser.h"
#include "nfa.h"
#include "dfa.h"
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REGEXNUM 23
#define GROUPNUM 4
#define SIZE 200
#define MAXGEN 2000
#define P_CORSS 0.75
#define P_MUTATION 0.05
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

int VERBOSE = 0;
int DEBUG = 0;
unsigned long DFAdata[REGEXNUM+1][REGEXNUM+1]={0};

struct groupnode
{

	int index[REGEXNUM];
	unsigned long fitness;
	unsigned long fitsum;

}cur[SIZE],next[SIZE],nodemax,nodemin;

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
static void cal_total_DFA(FILE *ruleset, regex_parser *parser){
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

/* init the data of 2-2 interact DFA */
bool init_data(FILE *ruleset, regex_parser *parser){

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
	for(i=2;i<=num;i++){
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

/* cal the fitness and fitsum of one node */
void cal_fitness(){

	int i,j,k;
	int count[GROUPNUM+1];
	int group[GROUPNUM+1][REGEXNUM];

	for(i=0;i<SIZE;i++){
		memset(count, 0, (GROUPNUM+1)*sizeof(int));
		memset(group, 0, (GROUPNUM+1)*REGEXNUM*sizeof(int));

		cur[i].fitness = 0;

		for(k=1;k<=GROUPNUM;k++){

			for(j=0;j<REGEXNUM;j++){				
				if(cur[i].index[j] == k){
					group[k][count[k]] = j + 1;
					count[k] = count[k] + 1;
				}
			}

			cur[i].fitness += cal_approximate_group_DFA(group[k],count[k]);
			cur[i].fitsum = i>0?(cur[i].fitness+cur[i-1].fitsum):(cur[0].fitness);
		}
		
		// for(k=1;k<=GROUPNUM;k++){
		// 	for(j=0;j<REGEXNUM;j++){	
		// 		printf("%u ", group[k][j]);
		// 	}
		// 	printf(">>%u \n",count[k]);
		// }
		// printf(">>%u >>%u \n",cur[i].fitness,cur[i].fitsum);
	}
}

/* init the first feneration */
void init_first_generation(){

	int i,j;

	for(i=0;i<SIZE;i++){

		for(j=0;j<REGEXNUM;j++){
			cur[i].index[j] =randi(GROUPNUM) ;
			//printf("%u ", cur[i].index[j]);
		}
		//printf("\n");

	}
	cal_fitness();
}



/* get which node to 换代  */
int sel(){
	double p=randd();
	double sum=cur[SIZE-1].fitsum;
	for(int i=0;i<SIZE;i++){
		if(cur[i].fitsum/sum>p) return i;
	}
}

/* 换代 */
void tran(){
	int i,j,k,pos;
    //找当前种群最优个体 
    nodemin=cur[0];
    for(i=1;i<SIZE;i++){
    	if(cur[i].fitness < nodemin.fitness)
    		nodemin=cur[i];   	
    }
    for(k=0;k<SIZE;k+=2){
    	//选择交叉个体 
    	i=sel();
   		j=sel();
     
    	//选择交叉位置 
    	pos=randi(REGEXNUM-1);
     
    	//交叉
    	if(randd()<P_CORSS){
    		memcpy(next[k].index,cur[i].index,pos *sizeof(int));
       		memcpy(next[k].index+pos,cur[j].index+pos,(REGEXNUM-pos)*sizeof(int));
 
    		memcpy(next[k+1].index,cur[j].index,pos*sizeof(int));
      		memcpy(next[k+1].index+pos,cur[i].index+pos,(REGEXNUM-pos)*sizeof(int));
     	}
    	else{
    		memcpy(next[k].index,cur[i].index,REGEXNUM*sizeof(int));
    		memcpy(next[k+1].index,cur[j].index,REGEXNUM*sizeof(int));
     	}
     	//变异
     	if(randd()<P_MUTATION){
       		pos=randi(REGEXNUM) - 1;
    		next[k].index[pos] = randi(GROUPNUM);
 
    		pos=randi(REGEXNUM) - 1;
    		next[k+1].index[pos] = randi(GROUPNUM);
    	}
   	}
   	//找下一代的最差个体 
  	nodemax=next[0],j=0;
  	for(i=1;i<SIZE;i++){
  		if(next[i].fitness > nodemax.fitness){
  			nodemax=next[i];
  	  		j=i;
  	  	}
   	}
   	//用上一代的最优个体替换下一代的最差个体
   	next[j]=nodemin;
   
   	memcpy(cur,next,sizeof(cur));
      
   	cal_fitness();
 }

void GA(){
	int i;
	int cnt=0;
	unsigned long ans;

	while(cnt++ < MAXGEN){
		tran();
	}

    nodemin=cur[0];
    for(i=1;i<SIZE;i++){
    	if(cur[i].fitness < nodemin.fitness)
    		nodemin=cur[i];   	
    }

	printf("minimal DFA is %u\n",nodemin.fitness);

	for(i=0;i<REGEXNUM;i++){
		printf("%u ", nodemin.index[i]);
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

	struct timeval start,middle,end;
	gettimeofday(&start,NULL);
	gettimeofday(&middle,NULL);
	/* BEGIN USER CODE */

	if(init_data(ruleset, parser)){
	
		//cal_total_DFA(ruleset, parser);

		srand((unsigned)time(0));

		init_first_generation();

		gettimeofday(&middle,NULL);

		printf(">> init time: %ldms\n",
			(middle.tv_sec-start.tv_sec)*1000+(middle.tv_usec-start.tv_usec)/1000);

		GA();

	}

	/* END USER CODE */
	gettimeofday(&end,NULL);

	printf(">> GA time: %ldms\n",
			(end.tv_sec-middle.tv_sec)*1000+(end.tv_usec-middle.tv_usec)/1000);

	if (parser!=NULL) delete parser;
	fclose(ruleset);
	/* Automata de-allocation */
	if (nfa!=NULL) delete nfa;
	if (dfa!=NULL) delete dfa;
		
	return 0;
}
