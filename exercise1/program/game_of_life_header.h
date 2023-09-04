void write_pgm_image(char *image, int maxval, int xsize, int ysize, const char *image_name);
void read_pgm_image( char **image, int *maxval, int *xsize, int *ysize, const char *image_name);
void random_playground(char* image, int xsize, int ysize);
int count_live_neighbors(char* image, int row, int col, int xsize, int ysize);
void ordered_evolution(char* image, int xsize, int ysize, int n, int s, char *destination_folder);
void static_evolution(char* image, int xsize, int ysize, int n, int s, char *destination_folder);