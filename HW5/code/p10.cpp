#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <cmath>
#include "linear.h"
#include <fstream>
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
#define INF HUGE_VAL

void print_null(const char *s) {}

void exit_input_error(int line_num)
{
	fprintf(stderr,"Wrong input format at line %d\n", line_num);
	exit(1);
}

static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input)
{
	int len;

	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}

// void parse_command_line(int argc, char **argv, char *input_file_name, char *model_file_name);
void read_problem(const char *filename);
// void do_cross_validation();
// void do_find_parameters();

struct feature_node *x_space;
struct parameter param;
struct problem prob;
struct model* model_;
struct model* model_1;
struct model* model_2;
struct model* model_3;
struct model* model_4;
struct model* model_5;
struct model* model_6;
int flag_cross_validation;
int flag_find_parameters;
int flag_C_specified;
int flag_p_specified;
int flag_solver_specified;
int nr_fold;
double bias;

struct result {
  double e_out;
  int non_zero_entries;
};

void init_params() {
	param.solver_type = 6;
	param.C = 1;
	param.p = 0.1;
	param.nu = 0.5;
	param.eps = 0.01; // see setting below
	param.nr_weight = 0;
	param.regularize_bias = 1;
	param.weight_label = NULL;
	param.weight = NULL;
	param.init_sol = NULL;
	flag_cross_validation = 0;
	flag_C_specified = 1;
	flag_p_specified = 0;
	flag_solver_specified = 1;
	flag_find_parameters = 0;
	bias = -1;
}

void free_prob() {
	free(prob.y);
	free(prob.x);
	free(x_space);
	free(line);
}

void train_model(int n) {
  param.C = pow(10, (double)(-n));
  if(n == -2) {
    model_1 = train(&prob, &param);
  }
  else if(n == -1) {
    model_2 = train(&prob, &param);
  }
  else if(n == 0) {
    model_3 = train(&prob, &param);
  }
  else if(n == 1) {
    model_4 = train(&prob, &param);
  }
  else if(n == 2) {
    model_5 = train(&prob, &param);
  }
  else if(n == 3) {
    model_6 = train(&prob, &param);
  }
  else {
    printf("wrong n\n");
  }
}

double calc_e(model *model_) {
  double e_in = 0;
  int l = prob.l;
  for(int i = 0; i < l; i++) {
    double prediction = predict(model_, prob.x[i]);
    if(prediction != prob.y[i]) e_in++;
  }
  return e_in;
}

int calc_non_zero_entries(model *mod_) {
  int count = 0;
  int w_length = mod_->nr_feature;
  for(int i = 0; i < w_length; i++) {
    if(mod_->w[i] != 0) count++;
  }
  
  return count;
}

result experiment() {

  char input_file_name[] = "./p10_data/train_data.txt";
  char test_file_name[] = "./p10_data/test_data.txt";
	read_problem(input_file_name);
  for(int i = -2; i <= 3; i++) {
    train_model(i);
  }
  // find best w
  double tmp;
  model_ = model_1;
  double mnm = calc_e(model_1);
  int best = 1;
  tmp = calc_e(model_2);
  if(tmp <= mnm) {
    model_ = model_2;
    mnm = tmp;
    best = 2;
  }
  tmp = calc_e(model_3);
  if(tmp <= mnm) {
    model_ = model_3;
    mnm = tmp;
    best = 3;
  }
  tmp = calc_e(model_4);
  if(tmp <= mnm) {
    model_ = model_4;
    mnm = tmp;
    best = 4;
  }
  tmp = calc_e(model_5);
  if(tmp <= mnm) {
    model_ = model_5;
    mnm = tmp;
    best = 5;
  }
  tmp = calc_e(model_6);
  if(tmp <= mnm) {
    model_ = model_6;
    mnm = tmp;
    best = 6;
  }
  free_prob();
  ///////////////////////////
  read_problem(test_file_name);
  printf("best log(lambda): %d\n", best - 3);
  double err = calc_e(model_) / 1990; 
  int non_zero_entries = calc_non_zero_entries(model_);

  free_and_destroy_model(&model_1);
  free_and_destroy_model(&model_2);
  free_and_destroy_model(&model_3);
  free_and_destroy_model(&model_4);
  free_and_destroy_model(&model_5);
  free_and_destroy_model(&model_6);
  free_prob();

  result res;
  res.e_out = err;
  res.non_zero_entries = non_zero_entries;
  return res;
}


int main(int argc, char **argv)
{
  // char model_file_name[] = "model_file.txt";

  // initialization
  set_print_string_function(&print_null);
  init_params();
  //
  // learn(1, model_file_name);
  // model_=train(&prob, &param);
  // save_model(model_file_name, model_);
  // print_model(model_);
  // for(int i = 0; i <= N; i++)
  //   printf("%f\n", w_1[i]);
  // free_and_destroy_model(&model_);

  // main part
  std::fstream data_out;
  std::fstream entries_out;
  data_out.open("./p10_data/p10.txt");
  entries_out.open("./p10_data/non_zero_entries.txt");
  for(int i = 0; i < 1126; i++) {
    printf("experiment number %d:\n", i);
    result res = experiment();
    printf("E_out : %f\n", res.e_out);
    data_out << res.e_out << '\n';
    entries_out << res.non_zero_entries << '\n';
  }
  data_out.close();
  entries_out.close();
  ///////////////////////

	destroy_param(&param);

  // for(int i = 0; i <= N; i++)
  //   printf("%f\n", w_4[i]);

	return 0;
}

// read in a problem (in libsvm format)
void read_problem(const char *filename)
{
	int max_index, inst_max_index, i;
	size_t elements, j;
	FILE *fp = fopen(filename,"r");
	char *endptr;
	char *idx, *val, *label;

	if(fp == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",filename);
		exit(1);
	}

	prob.l = 0;
	elements = 0;
	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	while(readline(fp)!=NULL)
	{
		char *p = strtok(line," \t"); // label

		// features
		while(1)
		{
			p = strtok(NULL," \t");
			if(p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
				break;
			elements++;
		}
		elements++; // for bias term
		prob.l++;
	}
	rewind(fp);

	prob.bias=bias;

	prob.y = Malloc(double,prob.l);
	prob.x = Malloc(struct feature_node *,prob.l);
	x_space = Malloc(struct feature_node,elements+prob.l);

	max_index = 0;
	j=0;
	for(i=0;i<prob.l;i++)
	{
		inst_max_index = 0; // strtol gives 0 if wrong format
		readline(fp);
		prob.x[i] = &x_space[j];
		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(i+1);

		prob.y[i] = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			exit_input_error(i+1);

		while(1)
		{
			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;

			errno = 0;
			x_space[j].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
				exit_input_error(i+1);
			else
				inst_max_index = x_space[j].index;

			errno = 0;
			x_space[j].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(i+1);

			++j;
		}

		if(inst_max_index > max_index)
			max_index = inst_max_index;

		if(prob.bias >= 0)
			x_space[j++].value = prob.bias;

		x_space[j++].index = -1;
	}

	if(prob.bias >= 0)
	{
		prob.n=max_index+1;
		for(i=1;i<prob.l;i++)
			(prob.x[i]-2)->index = prob.n;
		x_space[j-2].index = prob.n;
	}
	else
		prob.n=max_index;

	fclose(fp);
}
