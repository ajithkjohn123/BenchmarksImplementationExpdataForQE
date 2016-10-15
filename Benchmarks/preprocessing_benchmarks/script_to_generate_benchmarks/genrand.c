#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX_BITWIDTH 24

#define rand0to1 (((double) rand())/((double) RAND_MAX))

typedef unsigned long t_coeff; 

int gen_random_conj_constr(int nconstr, 
			   int nvars, 
			   int bitwidth,
			   double frac_diseq,
			   double frac_ineq, 
			   FILE *outfp,
			   char *outfilename);

main(int argc, char *argv[])
{
  if (argc != 7) {
    fprintf(stderr, 
	    "Usage: %s max_nconstr max_nvars bitwidth frac_diseq frac_ineq outfilename\n",
	    argv[0]);
    exit(-1);
  }

  /* Error handling not done fully for the following conversions */

  int max_nconstr = atoi(argv[1]);
  if (max_nconstr <= 0) {
    fprintf(stderr, "Error: max_nconstr (%d) must be > 0\n", max_nconstr);
    exit(-1);
  }

  int max_nvars = atoi(argv[2]);
  if (max_nvars <= 0) {
    fprintf(stderr, "Error: max_nvars (%d) must be > 0\n", max_nvars);
    exit(-1);
  }

  int bitwidth = atoi(argv[3]);
  if ((bitwidth <= 0) || (bitwidth > MAX_BITWIDTH))  {
    fprintf(stderr, "Error: bitwidth (%d) must be > 0 and <= %d\n", bitwidth, MAX_BITWIDTH);
    exit(-1);
  }

  double frac_diseq = atof(argv[4]);
  /*if ((frac_diseq <= 0.0) || (frac_diseq > 1.0)) {
    fprintf(stderr, "Error: frac_diseq (%g) must be > 0.0 and <= 1.0\n",
	    frac_diseq);
    exit(-1);
  }*/

  double frac_ineq = atof(argv[5]);
  /*if ((frac_ineq <= 0.0) || (frac_ineq > 1.0)) {
    fprintf(stderr, "Error: frac_ineq (%g) must be > 0.0 and <= 1.0\n",
	    frac_ineq);
    exit(-1);
  }*/
  
  FILE *outfp = fopen(argv[6], "w");
  if (outfp == NULL) {
    fprintf(stderr, "Could not open file %s for reading.\n", argv[4]);
    exit(-1);
  }

  char *benchmark_name = (char *) malloc(strlen(argv[6]) + 1);
  if (benchmark_name == NULL) {
    fprintf(stderr, "Error: Memory allocation failure.\n");
    exit(-1);
  }
  strcpy(benchmark_name, argv[6]);
  for (int i = 0, len = strlen(argv[6]); i < len; i++) {
    if (benchmark_name[i] == '.') {
      benchmark_name[i] = '_';
      break;
    }
  }
  
  srand(time(NULL));

  int nconstr;
  nconstr = max_nconstr;
  fprintf(stdout, "# Constraints: %d\n", nconstr);
  
  int nvars;
  nvars = max_nvars;
  fprintf(stdout, "# Vars: %d\n", nvars);
  
  fprintf(stdout, "Bitwidth of each var: %d\n", bitwidth);
  
  int stat = 
    gen_random_conj_constr(nconstr, 
			   nvars, 
			   bitwidth, 
			   frac_diseq,
			   frac_ineq, 
			   outfp,
			   benchmark_name);
  if (stat != 0) {
    fprintf(stderr, "Error: Failure in generating random conjunctive constraints.\n");
    exit(-1);
  }

  fclose(outfp);
  return(0);
}

int gen_random_conj_constr(int nconstr, 
			   int nvars, 
			   int bitwidth,
			   double frac_diseq,
			   double frac_ineq, 
			   FILE *outfp,
			   char *benchmark_name)
{
  if (outfp == NULL) {
    fprintf(stderr, "Cannot print to file with NULL file pointer\n");
    return(-1);
  }

  double temp = (frac_diseq * nconstr);
  int num_diseq_desired = (int) temp;
  if (temp - (double) num_diseq_desired >= 0.5) {
    num_diseq_desired++;
  }

  temp = (frac_ineq * nconstr);
  int num_ineq_desired = (int) temp;
  if (temp - (double) num_ineq_desired >= 0.5) {
    num_ineq_desired++;
  }

  t_coeff max_coeff = (1 << (bitwidth - 1)) + ((1 << (bitwidth - 1)) - 1);
  fprintf(stdout, "bitwidth: %d, max_coeff: %ld\n", 
	  bitwidth, (unsigned long) max_coeff);


  // Now start printing to outfp

  fprintf(outfp, "(benchmark random_%s\n", benchmark_name);

  for (int i = 0; i < nvars; i++) {
    fprintf(outfp, ":extrafuns ((x%d BitVec[%d]))\n",
	    i, bitwidth);
  }
  fprintf(outfp, "\n\n");

  
  fprintf(outfp, ":formula ");

  int diseq_count = 0;
  int ineq_count = 0;

  
  char *variable_used_flag_array = (char *) calloc(nvars, sizeof(char));
  if (variable_used_flag_array == NULL) {
    fprintf(stderr, "Error: Memory allocation failure.\n");
    return(-1);
  }

  for (int i = 0; i < nconstr; i++) {
    if ((nconstr > 1) && (i != nconstr - 1)) {
      if (i > 0) {
	fprintf(outfp, "\n         (and ");
      }
      else {
	fprintf(outfp, "(and ");
      }
    }
    else {
      if ((nconstr > 1) && (i == nconstr - 1)) {
	fprintf(outfp, "\n              ");
      }
    }

    char diseq_flag = 0;
    char ineq_flag = 0;

    // Choose whether this is an equality or disequality or inequality
    
    // The first num_diseq_desired constraints are disequalities
    if (i < num_diseq_desired) {
      diseq_flag = 1;
      diseq_count++;
    } // The next num_ineq_desired constraints are inequalities
    else if (i >= num_diseq_desired && i < num_diseq_desired + num_ineq_desired){
      ineq_flag = 1;
      ineq_count++;
    }
	

    if (diseq_flag == 1) {
      fprintf(outfp, "(not (= ");
    }
    else if (ineq_flag == 1) {
      fprintf(outfp, "(bvule      ");
    }
    else {
      fprintf(outfp, "(=      ");
    }

    t_coeff prev_nzero_coeff = 0;
    int prev_nzero_coeff_var_idx = 0;
    t_coeff curr_coeff = 0;
    int nzero_coeff_count = 0;
    char at_least_one_subterm_printed_flag = 0;
    
    do {
      for (int j = 0; j < nvars; j++) {
	// First decide whether the coeff of this var should be 0.
	// If it should be non-zero, choose a random non-zero coeff.
	if (((rand0to1) > 0.5) || 
	    ((i == nconstr - 1) && (variable_used_flag_array[j] == 0))) {

	  // If this is not the last constraint, flip a coin to decide.
	  // If this is the last constraint, check if this var has been
	  // used at all before.
	  do {
	    curr_coeff = (t_coeff) ((rand0to1) * max_coeff);
	  } while (curr_coeff == 0);
	  variable_used_flag_array[j] = 1;
	}
	else {
	  curr_coeff = 0;
	}

	if (curr_coeff != 0) {
	  if (prev_nzero_coeff != 0) {
	    if (at_least_one_subterm_printed_flag == 1) {
	      fprintf(outfp, "\n                        ");
	    }
	    fprintf(outfp, "(bvadd (bvmul bv%ld[%d] x%d) ",
		    prev_nzero_coeff, bitwidth, prev_nzero_coeff_var_idx);
	    at_least_one_subterm_printed_flag = 1;
	  }
	  prev_nzero_coeff = curr_coeff;
	  prev_nzero_coeff_var_idx = j;
	  nzero_coeff_count++;
	}
      }
    } while (nzero_coeff_count == 0);

    if (prev_nzero_coeff != 0) {
      fprintf(outfp, "(bvmul bv%ld[%d] x%d)",
	      prev_nzero_coeff, bitwidth, prev_nzero_coeff_var_idx);
    }

    for (int j = 1; j < nzero_coeff_count; j++) {
      fprintf(outfp, ")");
    }

    // Printing the constant 
    curr_coeff = (t_coeff) ((rand0to1) * max_coeff);
    fprintf(outfp, "\n\t              bv%ld[%d])", curr_coeff, bitwidth);

    if (diseq_flag == 1) {
      fprintf(outfp, ")");
    }
  }

  fprintf(stdout, "#Disequalities: %d, fraction: %g\n",
	  diseq_count, ((double) diseq_count)/((double) nconstr));

  fprintf(stdout, "#Inequaliies: %d, fraction: %g\n",
	  ineq_count, ((double) ineq_count)/((double) nconstr));
  
  fprintf(outfp, "\n\t ");
  for (int i = 1; i < nconstr; i++) {
    fprintf(outfp, ")");
  }

  fprintf(outfp, "\n\n)\n");

  return(0);
}
