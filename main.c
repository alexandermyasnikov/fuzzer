
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 12800

int main(int argc, char **argv) {

  char* arg_in = "-";
  char* arg_out = "-";
  char* arg_seed = "0";
  char* arg_factor = "100";
  int print = 0;

  int opt;
  while ((opt = getopt(argc, argv, "rwsfph")) != -1) {
    switch(opt) {
      case 'r': arg_in     = argv[optind]; break;
      case 'w': arg_out    = argv[optind]; break;
      case 's': arg_seed   = argv[optind]; break;
      case 'f': arg_factor = argv[optind]; break;
      case 'p': print = 1; break;
      case 'h':
        fprintf(stderr, "fuzzer version 1.0.0 \n");
        fprintf(stderr, "Usage: fuzzer [-r <file>] [-w <file>] [-s <seed>] [-f <factor>] [-p] [-h] \n");
        fprintf(stderr, "              -r:  Read the raw data from file. Standard input is used if file is '-'. \n");
        fprintf(stderr, "              -w:  Write the raw data to file. Standard output is used if file is '-'. \n");
        fprintf(stderr, "              -s:  Set seed for pseudo-random number generator. \n");
        fprintf(stderr, "              -f:  On average one of <factor> bytes will be random. \n");
        fprintf(stderr, "              -p:  Print diagnostics. \n");
        fprintf(stderr, "              -h:  Print the fuzzer version, usage message, and exit. \n");
        fprintf(stderr, "    Examples: \n");
        fprintf(stderr, "              cat my_dump | ./fuzzer \n");
        fprintf(stderr, "              cat my_dump | ./fuzzer -s 42 -f 1000 \n");
        fprintf(stderr, "              ./fuzzer -r my_dump_in -w my_dump_out \n");
        return 0;
      default: break;
    }
  }

  if (print) {
    fprintf(stderr, "in:     %s \n", arg_in);
    fprintf(stderr, "out:    %s \n", arg_out);
    fprintf(stderr, "seed:   %s \n", arg_seed);
    fprintf(stderr, "factor: %s \n", arg_factor);
  }

  FILE* in;
  FILE* out;
  unsigned int seed;
  unsigned int factor;

  if (!strcmp(arg_in, "-")) {
    in = stdin;
  } else {
    in = fopen(arg_in, "rb");
    if (!in) {
      fprintf(stderr, "can not open: '%s' \n", arg_in);
      return -1;
    }
  }

  if (!strcmp(arg_out, "-")) {
    out = stdout;
  } else {
    out = fopen(arg_out, "wb");
    if (!out) {
      fprintf(stderr, "can not open: '%s' \n", arg_out);
      return -1;
    }
  }

  seed = atoi(arg_seed);
  if (!seed) {
    seed = time(NULL);
  }

  factor = atoi(arg_factor);
  if (!factor) {
    factor = 1;
  }



  char buffer[BUFFER_SIZE];
  int size = 0;
  int stats_size = 0;
  int stats_factor = 0;

  srand(seed);
  int next_pos = rand() % factor;
  char next_val = rand();

  int offset = 0;

  while (1) {
    size = fread(buffer, 1, sizeof(buffer), in);

    if (!size)
      break;

    while (offset + next_pos < size) {
      buffer[offset + next_pos] = next_val;
      offset += 1 + 2 * next_pos; // Because mean of linear regression is 1/2;
      next_pos = rand() % factor;
      next_val = rand();
      stats_factor++;
    }
    stats_size += size;
    offset -= size;

    if (size != fwrite(buffer, 1, size, out))
      break;
  }

  fclose(in);
  fclose(out);

  if (print) {
    fprintf(stderr, "size:   %d \n", stats_size);
    fprintf(stderr, "factor: %d %f \n", stats_factor, 1. * stats_size / stats_factor);
  }

  return 0;
}

