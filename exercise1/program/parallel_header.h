#ifndef MY_FUNCTIONS
#define MY_FUNCTIONS 

void random_playground(unsigned char* image, int xsize, int ysize);
void write_pgm_image(unsigned char *image, int xsize, int ysize, int maxval, char* filename);
void read_header(int *xsize, int *ysize, int *maxval, char *filename);
void read_pgm_image(unsigned char* image, int xsize, int ysize, int maxval, char* filename);
unsigned char count_live_neighbors(unsigned char* image, int row, int col, int xsize, int ysize);
void ordered_evolution(unsigned char* image, int xsize, int ysize, int n, int s, char *destination_folder);
void static_upgrade(unsigned char* image, unsigned char* original_image, int xwidth, int ywidth, int x, int y);
void save_snapshot(unsigned char* image, int xwidth, int ywidth, int maxval, char* snap_title, int image_idx);

#endif