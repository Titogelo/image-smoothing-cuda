#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "IO.h"
#include "Filtros.h"
#include "cutil.h"

__global__ void Suma(float* A, float* B, float* C)
{
  int i = threadIdx.x;
  C[i] = A[i] + B[i];
}

int main(int argc, char* argv[]) {
   unsigned char *Cabecera=NULL;
   char          *Entrada=NULL;
   char          *Salida=NULL;

   unsigned int   nDimX, nDimY, CabeceraSize, rc, Itera, i;
   unsigned short BytePorPixel;
   float         **Imagen, *tImagen=NULL , *aImagen=NULL;
   float         **d_A, **d_B;

   if (argc != 4) {
      printf("Uso: Suaviza-Secuencial <Fichero-Entrada> <Fichero-Salida> Iteraciones\n");
      return -1;
   }

   Entrada = strdup(argv[1]);
   Salida  = strdup(argv[2]);
   Itera   = atoi(argv[3]);
   
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

   /* Para el Cuerpo de la Imagen */
   Imagen = Crea_Matriz(nDimY,nDimX);
   assert(Imagen);

   /* Vectores para almacenamiento temporal */
   tImagen = (float *)calloc(BytePorPixel*nDimX, sizeof(float));
   assert(tImagen);
   aImagen = (float *)calloc(BytePorPixel*nDimX, sizeof(float));
   assert(aImagen);

   /* Leer la imagen */
   rc = nLoad(Entrada, nDimX, nDimY, Imagen, Cabecera, CabeceraSize);
   assert(!rc);

   /* Reservar memoria y ponerla a cero en el device */
   unsigned int size_A = nDimX * nDimY;
   unsigned int mem_size_A = sizeof(float) * size_A;

   unsigned int size_B = nDimX * nDimY;
   unsigned int mem_size_B = sizeof(float) * size_B;
   
   CUDACreaMatriz(nDimY, nDimX, d_A);
   CUDACreaMatriz(nDimY, nDimX, d_B);

   cutilSafeCall(cudaMemcpy(d_A, Imagen, mem_size_A, cudaMemcpyHostToDevice));
   cutilSafeCall(cudaMemcpy(d_B, Imagen, mem_size_A, cudaMemcpyHostToDevice));




   
   /* Guardar la imagen */
   rc = nSave(Salida, nDimX, nDimY, Imagen, Cabecera, CabeceraSize);
   assert(!rc);
   
   free(Cabecera);
   Liberar_Matriz(Imagen, nDimY);
   free(tImagen);
   free(aImagen);

   return 0;
}
