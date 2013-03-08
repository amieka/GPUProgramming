
#include "Image.h"
#include <cuda.h>

//define the kernel function here
//kernel function for converting rgba to gray scale image
__global__
void rgba_to_greyscale(const u_char* const input,
                       u_char* output,
                       int numRows, int numCols)
{
    
   int col = blockIdx.y * blockDim.y + threadIdx.y;
   int row = blockIdx.x * blockDim.x + threadIdx.x;
   if ( row >= numRows || col >= numCols )
   {
      return;
   }
   	int offset_r = (row * numCols + col) * 3 + 1;
	int offset_g = (row * numCols + col) * 3 + 2;
	int offset_b = (row * numCols + col) * 3 + 3;
	float channelSum = .299f * input[offset_r] + .587f * input[offset_g] + .114f * input[offset_b];
	output[row * numCols + col] = channelSum;
}

void my_gray_scale(const u_char *input, u_char *output, int numRows, int numCols) {
	int row = 0, col = 0;
	for(row = 0; row < numRows; row++) {
		for(col = 0; col < numCols; col++) {
			int offset_r = (row * numCols + col) * 3 + 1;
			int offset_g = (row * numCols + col) * 3 + 2;
			int offset_b = (row * numCols + col) * 3 + 3;
			float channelSum = .299f * input[offset_r] + .587f * input[offset_g] + .114f * input[offset_b];
			output[row * numCols + col] = channelSum;
		}
	}
}

int main() {
	PPM *ppmImage = read_image("GreatWave.ppm");
	PPM *outImage = new PPM();
	int ppm_bytes = 0, pgm_bytes = 0;
	int numRows, numCols;
	u_char *d_gray = NULL;
	u_char *h_rgba = NULL;
	u_char *h_gray = NULL;
	u_char *rgba = NULL;
	if(!ppmImage) {
		printf("there was an error reading the image");
	}
	numRows = ppmImage->height;
	numCols = ppmImage->width;
	outImage->height = ppmImage->height;
	outImage->width = ppmImage->width;
	
	//printf("image data %d %d ", numRows, numCols);
	
	ppm_bytes = numCols * numRows * sizeof(u_char) * 3;
	pgm_bytes = numCols * numRows * sizeof(u_char);
	
	//allocate some memory to host data structures
	h_rgba = ppmImage->data;
	h_gray = (u_char *)malloc(pgm_bytes); // allocate some memory to the output image
	
	//allocate memory to device data structures
	cudaMalloc((void**)&d_gray, pgm_bytes);
	cudaMalloc((void**)&rgba, ppm_bytes);
	
	
	//memcpy stuff from host to device array 
	cudaMemcpy(rgba, h_rgba, ppm_bytes, cudaMemcpyHostToDevice);
	cudaMemcpy(d_gray, h_gray, ppm_bytes, cudaMemcpyHostToDevice);
	
	
	//set the block and grid size
	
	const dim3 blockSize(1,1);  //set a block size of 1
	const dim3 gridSize(numRows , numCols );  	
	
	//run the kernel function for converting rgba into gray scale image
	
	rgba_to_greyscale<<<gridSize, blockSize>>>(rgba, d_gray, numRows, numCols);
	//my_gray_scale(rgba, d_gray, numRows, numCols);
	
	//copy back data from device to host again
	
	cudaMemcpy(h_gray, d_gray, pgm_bytes, cudaMemcpyDeviceToHost);
	
	outImage->data = h_gray;
	//write out to a pgm file here
	printf("saving the pgm image here \n");
	save_pgm_image(outImage, "out.pgm");
	
	//printf("height %d\n", ppmImage->height);
	return 0;
}