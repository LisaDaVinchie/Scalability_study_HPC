void random_playground(char* image, int xsize, int ysize)
{
    int idx = 0;
    for (int y = 0; y < ysize; y++){
        for (int x = 0; x < xsize; x++){
            // printf("%d\n", (int)((double)rand()/(double)RAND_MAX+0.5));
            image[idx] = (char)((double)rand()/(double)RAND_MAX+0.5);
            if (image[idx] == 1){
              image[idx] = (char)255;
            }
            
            printf("%c ", image[idx] - 207 );
            idx++;
        }
    printf("\n");
    }
}