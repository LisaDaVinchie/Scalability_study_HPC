void * generate_gradient( int maxval, int xsize, int ysize )
/*
 * just and example about how to generate a vertical gradient
 * maxval is either 255 or 65536, xsize and ysize are the
 * x and y dimensions of the image to be generated.
 * The memory region that will contain the image is returned
 * by the function as a void *

 */
{
  char      *cImage;   // the image when a single byte is used for each pixel
  short int *sImage;   // the image when a two bytes are used for each pixel
  void      *ptr;
  
  int minval      = 0; 
  int delta       = (maxval - minval) / ysize;
  
  if(delta < 1 )
    delta = 1;
  
  if( maxval < 256 )
    // generate a gradient with 1 byte of color depth
    {
      cImage = (char*)calloc( xsize*ysize, sizeof(char) );
      unsigned char _maxval = (char)maxval;
      int idx = 0;
      for ( int yy = 0; yy < ysize; yy++ )
	{
	  unsigned char value = minval + yy*delta;
	  for( int xx = 0; xx < xsize; xx++ )
	    cImage[idx++] = (value > _maxval)?_maxval:value;
	}
      ptr = (void*)cImage;
    }
  else
    // generate a gradient with 2 bytes of color depth
    {
      sImage = (unsigned short int*)calloc( xsize*ysize, sizeof(short int) );
      unsigned short int _maxval = swap((unsigned short int)maxval);
      int idx = 0;
      for ( int yy = 0; yy < ysize; yy++ )
	{
	  unsigned short int value  = (short int) (minval+ yy*delta);
	  unsigned short int _value = swap( value );    // swap high and low bytes, the format expect big-endianism
	  
	  for( int xx = 0; xx < xsize; xx++ )
	    sImage[idx++] = (value > maxval)?_maxval:_value;
	}
      ptr = (void*)sImage;	
    }

  return ptr;
}




int main( int argc, char **argv ) 
{ 
    int xsize      = XWIDTH;
    int ysize      = YWIDTH;
    int maxval     = MAXVAL;

    // print information about endianism
    printf("this machine is %s\n", (I_M_LITTLE_ENDIAN)?"little endian":"big endian");

    // you can use also the system-defined macro LITTLE_ENDIAN
    printf("2nd check: this machine definitely is %s\n", (LITTLE_ENDIAN)?"little endian":"big endian");
    
    if ( argc > 1 )
      {
	maxval = atoi( *(argv+1) ) % 65536;
	if ( argc > 3 )
	  {
	    xsize = atoi( *(argv+2) );
	    ysize = atoi( *(argv+2) );
	  }
      }

    // ---------------------------------------------------
    //
    void *ptr = generate_gradient( maxval, xsize, ysize );
    printf("The gradient has been generated\n");

    // ---------------------------------------------------
    //    
    write_pgm_image( ptr, maxval, xsize, ysize, "image.pgm" );
    printf("The gradient has been written\n");
    
    free(ptr);

    // ---------------------------------------------------
    //
    
    xsize = 0;
    ysize = 0;
    maxval = 0;
    
    read_pgm_image( &ptr, &maxval, &xsize, &ysize, "image.pgm");
    printf("The gradient has been read in again\n");
    
    free( ptr );

    read_pgm_image( &ptr, &maxval, &xsize, &ysize, "check_me.pgm");
    printf("The imaget has been read\n");

    // swap the endianism
    //
    if ( I_M_LITTLE_ENDIAN )
      swap_image( ptr, xsize, ysize, maxval);

    // do something on the image (for instance, blur it)
    // ...
    //
    
    // swap the endianism
    //
    if ( I_M_LITTLE_ENDIAN )
      swap_image( ptr, xsize, ysize, maxval);
        
    write_pgm_image( ptr, maxval, xsize, ysize, "check_me.back.pgm");
    printf("The imaget has been written back\n");

    free(ptr);
    return 0;
} 