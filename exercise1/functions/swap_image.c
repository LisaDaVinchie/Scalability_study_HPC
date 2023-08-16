void swap_image( void *image, int xsize, int ysize, int maxval )
/*
 * This routine swaps the endianism of the memory area pointed
 * to by ptr, by blocks of 2 bytes
 *
 */
{
  if ( maxval > 255 )
    {
      // pgm files has the short int written in
      // big endian;
      // here we swap the content of the image from
      // one to another
      //
      unsigned int size = xsize * ysize;
      for ( int i = 0; i < size; i++ )
  	((unsigned short int*)image)[i] = swap(((unsigned short int*)image)[i]);
    }
  return;
}