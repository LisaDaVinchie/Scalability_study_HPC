#ifndef MY_FUNCTIONS
#define MY_FUNCTIONS

void write_pgm_image(unsigned char *image, int xsize, int ysize, int maxval, char* filename);

void read_header(int *xsize, int *ysize, int *maxval, char *filename);

void read_pgm_image(unsigned char* image, int xsize, int ysize, int maxval, char* filename);

void ordered_evolution(unsigned char* image, int xsize, int ysize, int n, int s, char *destination_folder);

void save_snapshot(unsigned char* image, int xwidth, int ywidth, int maxval, char* snap_title, int first_index, int image_idx);

void print_matrix(unsigned char* image, int xwidth, int ywidth);

void gather_images(unsigned char* partial_image, unsigned char* full_image, int xwidth, int ywidth, int n_procs, int rank);

void initialise_playground(int k, int maxval, char* fname, int argc, char **argv);



unsigned char count_live_neighbors(unsigned char* image, int row, int col, int xwidth, int ywidth);

int static_upgrade(unsigned char* image, unsigned char* original_image, int xwidth, int ywidth, int row, int col);

void static_evolution(char* fname, int n, int s, int argc, char **argv);



unsigned char count_live_neighbors_v2(unsigned char* image, int row, int col, int xwidth, int ywidth);

int static_upgrade_v2(unsigned char* image, int xwidth, int ywidth, int row, int col);

void static_evolution_v2(char* fname, int n, int s, int argc, char **argv);

#endif