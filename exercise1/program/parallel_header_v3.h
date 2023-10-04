#ifndef MY_FUNCTIONS
#define MY_FUNCTIONS

void write_pgm_image(unsigned char *image, int xsize, int ysize, int maxval, char* filename);

void read_header(int *xsize, int *ysize, int *maxval, char *filename);

void read_pgm_image(unsigned char* image, int xsize, int ysize, int maxval, char* filename);

void print_matrix(unsigned char* image, int xwidth, int ywidth);

void gather_images(unsigned char* partial_image, unsigned char* full_image, int xwidth, int ywidth, int n_procs, int rank);

void initialise_playground(int kx, int ky, int maxval, char* fname, int argc, char **argv);



void savePGM(const char* filename, unsigned char* image, int xwidth, int rows_per_proc, int maxval);
void save_snapshot(unsigned char* image, int xwidth, int rows_per_proc, int maxval, char* snap_title, int first_index, int image_idx);


unsigned char count_live_neighbors(unsigned char* image, int row, int col, int xwidth, int ywidth);


int ordered_upgrade(unsigned char* image, int xwidth, int ywidth, int row, int col);

void ordered_evolution(char* fname, int n, int s, int argc, char **argv);




int static_upgrade(unsigned char* aux_image, unsigned char* image, int xwidth, int ywidth, int row, int col);

void static_evolution(char* fname, int n, int s, int argc, char **argv);

#endif