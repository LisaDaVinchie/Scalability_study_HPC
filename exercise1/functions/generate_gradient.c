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