
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct PPM
{
  int width;
  int height;
  u_char *data;
};

typedef enum {
	kPGMImage = 1 << 0,
	kPPMImage = 1 << 1
}ImageType;

PPM *create_image(int width, int height);
PPM *read_image(char *filename);
void save_image(PPM *image, char *filename, int image_type);
void save_pgm_image(PPM *image, char *filename);
static void readPPMHeader(FILE *fp, int *width, int *height);
void set_pixel(PPM *image, int x, int y, int chan, u_char val);
void clear_image(PPM *image, u_char red, u_char green, u_char blue);

