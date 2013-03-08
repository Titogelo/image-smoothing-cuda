#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "IO.h"

int main(int argc, char* argv[]) {
   FILE
      *fp1, *fp2;
   
   unsigned char
      *Cabecera1=NULL, *Cabecera2=NULL;
      
   char
      *Fichero1=NULL, *Fichero2=NULL;

   unsigned char
      caracter1, caracter2;

   unsigned int
      nDimX1, nDimY1, CabeceraSize1;
      
   unsigned int
      nDimX2, nDimY2, CabeceraSize2;
   
   unsigned int
      i, j, k, rc;

   unsigned short
      BytePorPixel1, BytePorPixel2;

   if (argc != 3) {
      printf("Uso: %s <Fichero-1> <Fichero-2>\n", argv[0]);
      return -1;
   }

   Fichero1 = strdup(argv[1]);
   Fichero2 = strdup(argv[2]);
   
   /* Obtener caracteristicas de la imagen 1*/
   rc = nGetSize(Fichero1, &nDimX1, &nDimY1, &BytePorPixel1, &CabeceraSize1);
   assert(!rc);

   /* Obtener caracteristicas de la imagen 2*/
   rc = nGetSize(Fichero2, &nDimX2, &nDimY2, &BytePorPixel2, &CabeceraSize2);
   assert(!rc);

   /* Que los datos iniciales sean iguales */
   if (nDimX1 != nDimX2)
   {
      printf("La dimension X de las imagenes no coinciden\n");
      return -1;   
   }
   if (nDimY1 != nDimY2)
   {
      printf("La dimension Y de las imagenes no coinciden\n");
      return -1;   
   }
   if (BytePorPixel1 != BytePorPixel2)
   {
      printf("Los bytes por pixel de las imagenes no coinciden\n");
      return -1;   
   }
   if (CabeceraSize1 != CabeceraSize2)
   {
      printf("Los tamas de las cabeceras de las imagenes no coinciden\n");
      return -1;   
   }

   /* Para la Cabecera de la Imagen 1*/
   Cabecera1 = (unsigned char *)calloc(CabeceraSize1, sizeof(unsigned char));
   assert(Cabecera1);

   /* Para la Cabecera de la Imagen 2*/
   Cabecera2 = (unsigned char *)calloc(CabeceraSize2, sizeof(unsigned char));
   assert(Cabecera2);

   /* Saltar las cabeceras, no las comprobamos ??? */
   fp1 = fopen(Fichero1, "rb");
   assert(fp1);  
   fread(Cabecera1, CabeceraSize1, 1, fp1);

   fp2 = fopen(Fichero2, "rb");
   assert(fp2);
   fread(Cabecera2, CabeceraSize2, 1, fp2);

   /* Leer Byte a Byte y comparando */
   printf("Dimensiones: Y=%u, X=%u\n", nDimY1, nDimX1);
   
   for (i=0; i<nDimY1; i++)
      for (j=0; j<nDimX1; j++)
         for (k=0; k<BytePorPixel1; k++)
         {
           fread(&caracter1, 1, 1, fp1);
           fread(&caracter2, 1, 1, fp2);
           if ((caracter1 != caracter2) && (abs(caracter1-caracter2) > 1))
              printf("Diferencia en el byte de posicion (%u, %u, %u) con valores (%d, %d)\n", i,j,k, caracter1, caracter2);
         }
   fclose(fp1);
   fclose(fp2);
   return 0;
}
