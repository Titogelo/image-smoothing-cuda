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
   float         **Imagen, *tImagen=NULL , *aImagen=NULL;

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
   Imagen = Crea_Matriz(nDimY+2,nDimX+2);
   assert(Imagen);

   /* Vectores para almacenamiento temporal */
   tImagen = (float *)calloc(BytePorPixel*(nDimX+2), sizeof(float));
   assert(tImagen);
   aImagen = (float *)calloc(BytePorPixel*(nDimX+2), sizeof(float));
   assert(aImagen);

   /* Leer la imagen */
   rc = nLoad2(Entrada, nDimX, nDimY, Imagen, Cabecera, CabeceraSize);
   assert(!rc);

   for (i=1; i<=Itera; i++)
      Suaviza2(Imagen, tImagen, aImagen, nDimX, nDimY);
   
   /* Guardar la imagen */
   rc = nSave2(Salida, nDimX, nDimY, Imagen, Cabecera, CabeceraSize);
   assert(!rc);
   
   free(Cabecera);
   Liberar_Matriz(Imagen, nDimY+2);
   free(tImagen);
   free(aImagen);

   return 0;
}
