#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "IO.h"
#include <cutil_inline.h>

//Device Code
__global__ void Binariza(float *Tratada, float *Original, unsigned int nDimX, unsigned int nDimY, float Media)
{
	unsigned int i;

	// Creamos el indice del hilo
	i = blockDim.x * blockIdx.x + threadIdx.x;
	
	if( i <= (nDimX * nDimY-1) ){
		if (Original[i] <= Media)
			Tratada[i]=255;
		else
			Tratada[i]=0;
	}
}





int main(int argc, char* argv[]) {
   unsigned char *Cabecera=NULL;
   char          *Entrada=NULL;
   char          *Salida=NULL;

   unsigned int   nDimX, nDimY, CabeceraSize, rc, Itera, i;
   unsigned short BytePorPixel;

   int HilosPorBloque=0, BloquesPorGrid=0, media=0;

   float *ImagenTratada=NULL,    *ImagenOriginal=NULL;
   float *GPUTratada=NULL, *GPUOriginal=NULL;
   float time=.0; 

   cudaEvent_t start, stop;

   if (argc != 6) {
      printf("Uso: Suaviza-Secuencial <Fichero-Entrada> <Fichero-Salida> <Iteraciones> <Threads per Block> <Media>\n");
      return -1;
   }

   Entrada        = strdup(argv[1]);
   Salida         = strdup(argv[2]);
   Itera          = atoi(argv[3]);
   HilosPorBloque = atoi(argv[4]);
   media 		  = atoi(argv[5]);
   
   /* Obtener caracteristicas de la imagen */
   rc = nGetSize(Entrada, &nDimX, &nDimY, &BytePorPixel, &CabeceraSize);
   assert(!rc);

   /* Asegurando que es 1 Byte por Pixel */
   if (BytePorPixel != 1) {
      printf("Lo sentimos pero solo para 1 Byte por pixel\n");
      return -1;
   }

   /* Para la Cabecera de la Imagen */
   Cabecera = (unsigned char *)calloc(CabeceraSize, sizeof(unsigned char));
   assert(Cabecera);

   /* Almacena la Imagen que se esta actualizado (t+1) */
   ImagenTratada = (float *)calloc(BytePorPixel*nDimX*nDimY, sizeof(float));
   assert(ImagenTratada);

   /* Almacena la Imagen que se usa para actualizar en (t+1), esto es, la imagen en (t) */
   ImagenOriginal  = (float *)calloc(BytePorPixel*nDimX*nDimY, sizeof(float));
   assert(ImagenOriginal);

   /* Leer la imagen */
   rc = nLoad3(Entrada, nDimX, nDimY, ImagenOriginal, Cabecera, CabeceraSize);
   assert(!rc);

   /* No usar llamadas a cuda antes de usar setdevice. */
   //cutilSafeCall(cudaSetDevice(0));

   cudaEventCreate(&start);
   cudaEventCreate(&stop);

   // Reservando memoria para los vectores en el device
   // cutilSafeCall( );
   cudaMalloc((void**)&GPUOriginal,  nDimX*nDimY*sizeof(float));
   cudaMalloc((void**)&GPUTratada, nDimX*nDimY*sizeof(float));

   //cutilSafeCall(
   cudaMemcpy(GPUOriginal, ImagenOriginal, nDimX*nDimY*sizeof(float), cudaMemcpyHostToDevice);

   BloquesPorGrid= (nDimX*nDimY + HilosPorBloque -1) / HilosPorBloque;

   cudaEventRecord(start,0);
	printf("comienza");
   for (i=1; i<=Itera; i++)
   {
      if ((i % 2) != 0)
         Binariza<<<BloquesPorGrid, HilosPorBloque>>>(GPUTratada,  GPUOriginal,  nDimX, nDimY, media);
      else
         Binariza<<<BloquesPorGrid, HilosPorBloque>>>(GPUOriginal, GPUTratada, nDimX, nDimY, media);
   }
   printf("acaba");
   //cutilCheckMsg("kernel launch failure");
   
   cudaEventRecord(stop,0);
   cudaEventSynchronize(stop);
   cudaEventElapsedTime(&time,start,stop);

   printf("tiempo %f\n", time);

   if ((Itera % 2) != 0)
      // cutilSafeCall();
 	cudaMemcpy(ImagenTratada, GPUTratada, nDimX*nDimY*sizeof(float), cudaMemcpyDeviceToHost);
   else
      // cutilSafeCall();
	cudaMemcpy(ImagenTratada, GPUOriginal,  nDimX*nDimY*sizeof(float), cudaMemcpyDeviceToHost);

   /* Guardar la imagen */
   rc = nSave3(Salida, nDimX, nDimY, ImagenTratada, Cabecera, CabeceraSize);
   assert(!rc);
   
   free(Cabecera);
   free(ImagenTratada);
   free(ImagenOriginal);

//   cutilSafeCall();
	cudaFree(GPUTratada);
   	cudaFree(GPUOriginal);
   cudaEventDestroy(start);
   cudaEventDestroy(stop);  

   return 0;
}
