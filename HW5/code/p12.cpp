#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <cmath>
#include <fstream>
#include <random>
#include <set>
#include "linear.h"
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
int flag_cross_validation;
int flag_find_parameters;
int flag_C_specified;
int flag_p_specified;
int flag_solver_specified;
int nr_fold = 3;
double bias;

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
	flag_cross_validation = 1;
	flag_C_specified = 1;
	flag_p_specified = 0;
	flag_solver_specified = 1;
	flag_find_parameters = 0;
	bias = 1;
}

void free_prob() {
	free(prob.y);
	free(prob.x);
	free(x_space);
	free(line);
}

double cross_validation_error(int n) {
  param.C = pow(10, (double)(-n));
  double *target = Malloc(double, prob.l);
  int correct = 0;

  cross_validation(&prob, &param, nr_fold, target);
  for(int i = 0; i < prob.l; i++) {
    if(target[i] == prob.y[i]) correct++;
  }

  free(target);
  return (double)correct / (double)prob.l;
}

struct result {
  double e_out;
  int non_zero_entries;
  int best_lambda;
};

double calc_e(model *model_) {
  double e_in = 0;
  int l = prob.l;
  for(int i = 0; i < l; i++) {
    double prediction = predict(model_, prob.x[i]);
    if(prediction != prob.y[i]) e_in++;
  }
  return e_in;
}

result experiment() {
  char train_data[] = "./p12_data/train_data.txt";
  char test_data[] = "./p12_data/test_data.txt";

  read_problem(train_data);

  int best = -2;
  double mxm = cross_validation_error(-2);

  for(int i = -1; i <= 3; i++) {
    double tmp = cross_validation_error(i);
    if(tmp >= mxm) {
      mxm = tmp;
      best = i;
    }
  }

  printf("best log(lambda): %d\n", best);

  param.C = pow(10, (double)(-best));
  model_ = train(&prob, &param);

  free_prob();
  read_problem(test_data);
  
  double err = calc_e(model_);

  free_and_destroy_model(&model_);
  free_prob();

  result res;
  res.e_out = err / 1990;
  res.best_lambda = best; 

  return res;
}

int main(int argc, char **argv)
{
  // initialization
  set_print_string_function(&print_null);
  init_params();
  
  // main part
  std::fstream data_out;
  std::fstream lambda_out;
  data_out.open("./p12_data/p12.txt");
  lambda_out.open("./p12_data/best_lambda.txt");
  for(int i = 0; i < 1126; i++) {
    std::cout << "experiment number " << i << ":\n";
    result res = experiment();
    double err = res.e_out;
    data_out << err << '\n';
    lambda_out << res.best_lambda << '\n';
    printf("E_out : %f\n", err);
  }
  data_out.close();
  ////////////////////////////////

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
