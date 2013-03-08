#include "Image.h"


static void
checkDimension(int dim)
{
  if (dim < 1 || dim > 4000) {
	printf("file contained unreasonable width or height");
	return ;
  } 
}

static void
readPPMHeader(FILE *fp, int *width, int *height)
{
  char ch;
  int  maxval;

  if (fscanf(fp, "P%c\n", &ch) != 1 || ch != '6') {
	printf("file is not in ppm raw format; cannot read");
	return ;
  }

  /* skip comments */
  ch = getc(fp);
  while (ch == '#')
    {
      do {
	ch = getc(fp);
      } while (ch != '\n');	/* read to the end of the line */
      ch = getc(fp);            /* thanks, Elliot */
    }

  if (!isdigit(ch)) {
	printf("cannot read header information from ppm file");
	return ;
  }
  ungetc(ch, fp);		/* put that digit back */

  /* read the width, height, and maximum value for a pixel */
  fscanf(fp, "%d%d%d\n", width, height, &maxval);

  if (maxval != 255) {
	printf("image is not true-color (24 bit); read failed");
	return ;
  }
  checkDimension(*width);
  checkDimension(*height);
}

PPM *create_image(int width, int height) {
	//create a ppm image here
	PPM *image = (PPM *) malloc(sizeof(PPM));

	if (!image) printf("cannot allocate memory for new image");

  	image->width  = width;
  	image->height = height;
  	image->data   = (u_char *) malloc(width * height * 3);

  	if (!image->data) printf("cannot allocate memory for new image");

  	return image;
}

PPM *read_image(char *filename) {
	//read a ppm image here
	int width, height, num, size;
	  u_char *p;

	  PPM *image = (PPM *) malloc(sizeof(PPM));
	  FILE  *fp    = fopen(filename, "r");

	  if (!image) printf("cannot allocate memory for new image");
	  if (!fp)    printf("cannot open file for reading");

	  readPPMHeader(fp, &width, &height);

	  size          = width * height * 3;
	  image->data   = (u_char *) malloc(size);
	  image->width  = width;
	  image->height = height;

	  if (!image->data) printf("cannot allocate memory for new image");

	  num = fread((void *) image->data, 1, (size_t) size, fp);

	  if (num != size) printf("cannot read image data from file");

	  fclose(fp);

	  return image;
}

void  save_image(PPM *image, char *filename, int image_type) {
	//write a ppm image here
	int num;
	int size;
	switch(image_type) {
		case 1:
			size = image->width * image->height;
			break;
		case 2:
			size = image->width * image->height * 3;
			break;
		default:
			break;
	}
	
	FILE *fp = fopen(filename, "w");

	if (!fp) {
		printf("cannot open file for writing");
		return ;
	}
	fprintf(fp, "P6\n%d %d\n%d\n", image->width, image->height, 255);

	num = fwrite((void *) image->data, 1, (size_t) size, fp);

	if (num != size) {
		printf("cannot write image data to file");
		return;
	}

	fclose(fp);
}

void save_pgm_image(PPM *image, char *filename) {
	int num;
	int size = image->width * image->height;

	FILE *fp = fopen(filename, "w");

	if (!fp) {
		printf("cannot open file for writing");
		return ;
	}
	fprintf(fp, "P5\n%d %d\n%d\n", image->width, image->height, 255);

	num = fwrite((void *) image->data, 1, (size_t) size, fp);

	if (num != size) {
		printf("cannot write image data to file");
		return;
	}

	fclose(fp);
}

void set_pixel(PPM *image, int x, int y, int chan, u_char val)
{
  int offset = (y * image->width + x) * 3 + chan;

  image->data[offset] = val;
}


void clear_image(PPM *image, u_char red, u_char green, u_char blue)
{
  int i;
  int pix = image->width * image->height;

  u_char *data = image->data;

  for (i = 0; i < pix; i++)
    {
      *data++ = red;
      *data++ = green;
      *data++ = blue;
    }
}

