#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "IO.h"
#include "Filtros.h"

int main(int argc, char* argv[]) {
   unsigned char *Cabecera=NULL;
   float        **Imagen=NULL;
   char          *Entrada=NULL;
   char          *Salida=NULL;

   unsigned int nDimX, nDimY, CabeceraSize, rc;
   unsigned short BytePorPixel;

   if (argc != 3) {
      printf("Uso: Binariza-Secuencial <Fichero-Entrada> <Fichero-Salida>\n");
      return -1;
   }

   Entrada = strdup(argv[1]);
   Salida  = strdup(argv[2]);
   
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

   /* Leer la imagen */
   rc = nLoad(Entrada, nDimX, nDimY, Imagen, Cabecera, CabeceraSize);
   assert(!rc);

   Negativiza(Imagen, nDimX, nDimY);

   /* Guardar la imagen */
   rc = nSave(Salida, nDimX, nDimY, Imagen, Cabecera, CabeceraSize);
   assert(!rc);

   Liberar_Matriz(Imagen, nDimY);
   free(Cabecera);

   return 0;
}
