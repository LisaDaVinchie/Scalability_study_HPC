#ifndef INITIALISE
#define INITIALISE

void write_pgm_image_parallel(unsigned char* image, int xwidth, int ywidth, int rows_per_proc, int rank, int maxval, const char* filename);

void initialise_playground(int k, int maxval, char* fname, int argc, char **argv);

#endif