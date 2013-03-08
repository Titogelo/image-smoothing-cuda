#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "IO.h"
#include "Filtros.h"

int main(int argc, char* argv[]) {
   unsigned char *Cabecera=NULL;
   char          *Entrada=NULL;
   char          *Salida=NULL;

   unsigned int   nDimX, nDimY, CabeceraSize, rc, Itera, i;
   unsigned short BytePorPixel;
   float          *Imagen, *tImagen=NULL , *aImagen=NULL;

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
   Imagen = (float *)calloc(BytePorPixel*nDimX*nDimY, sizeof(float));
   assert(Imagen);

   /* Vectores para almacenamiento temporal */
   tImagen = (float *)calloc(BytePorPixel*nDimX, sizeof(float));
   assert(tImagen);

   aImagen = (float *)calloc(BytePorPixel*nDimX, sizeof(float));
   assert(aImagen);

   /* Leer la imagen */
   rc = nLoad3(Entrada, nDimX, nDimY, Imagen, Cabecera, CabeceraSize);
   assert(!rc);

   for (i=1; i<=Itera; i++)
      Suaviza1D(Imagen, tImagen, aImagen, nDimX, nDimY);
   
   /* Guardar la imagen */
   rc = nSave3(Salida, nDimX, nDimY, Imagen, Cabecera, CabeceraSize);
   assert(!rc);
   
   free(Cabecera);
   free(Imagen);
   free(tImagen);
   free(aImagen);

   return 0;
}
