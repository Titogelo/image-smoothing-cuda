#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "IO.h"
#include <cutil_inline.h>

//Device Code
__global__ void SuavizaCuda(float *Tratada, float *Original, unsigned int nDimX, unsigned int nDimY)
{
  unsigned int
      UpIzq, UpCen, UpDer,
      Izqda, Centr, Derch,
      DoIzq, DoCen, DoDer,
      UltPriFil, PriUltFil, UltUltFil,
      PriCol, UltCol;
   
   float
      fUpIzq, fUpCen, fUpDer,
      fIzqda, fCentr, fDerch,
      fDoIzq, fDoCen, fDoDer,
      Divisor;
 


	// Creamos el indice del hilo
   int i = blockDim.x * blockIdx.x + threadIdx.x;

   UltPriFil=nDimX-1;
   PriUltFil=(nDimY-1)*nDimX;
   UltUltFil=nDimY*nDimX-1;
   
   if( i <= (nDimX * nDimY-1) ){
  
      UpCen=i-nDimX; UpIzq=UpCen-1; UpDer=UpCen+1;
      Centr=i;       Izqda=Centr-1; Derch=Centr+1;
      DoCen=i+nDimX; DoIzq=DoCen-1; DoDer=DoCen+1;

      PriCol=i     % nDimX;
      UltCol=(i+1) % nDimX;
   
      if (i == 0) /* elemento (0,0) */
      {
         fUpIzq=.0; fUpCen=.0;            fUpDer=.0;
         fIzqda=.0; fCentr=Original[Centr]; fDerch=Original[Derch];
         fDoIzq=.0; fDoCen=Original[DoCen]; fDoDer=Original[DoDer];
         Divisor=4.0;
      }
      else
      {
         if (i == UltPriFil) /* elemento (0, nDimX-1) */
         {
            fUpIzq=.0;            fUpCen=.0;            fUpDer=.0;
            fIzqda=Original[Izqda]; fCentr=Original[Centr]; fDerch=.0;
            fDoIzq=Original[DoIzq]; fDoCen=Original[DoCen]; fDoDer=.0;
            Divisor=4.0;
         }
         else
         {
            if (i == PriUltFil) /* elemento (nDimY-1, 0) */
            {
               fUpIzq=.0; fUpCen=Original[UpCen]; fUpDer=Original[UpDer];
               fIzqda=.0; fCentr=Original[Centr]; fDerch=Original[Derch];
               fDoIzq=.0; fDoCen=.0;            fDoDer=.0;
               Divisor=4.0;
            }
            else
            {
               if (i == UltUltFil) /* elemento (nDimY-1, nDimX-1) */ 
               {
                  fUpIzq=Original[UpIzq]; fUpCen=Original[UpCen]; fUpDer=.0;
                  fIzqda=Original[Izqda]; fCentr=Original[Centr]; fDerch=.0;
                  fDoIzq=.0;            fDoCen=.0;            fDoDer=.0;
                  Divisor=4.0;
               }
               else 
               {
                  if (i < UltPriFil) /* elementos intermedios primera fila */
                  {
                     fUpIzq=.0;            fUpCen=.0;            fUpDer=.0;
                     fIzqda=Original[Izqda]; fCentr=Original[Centr]; fDerch=Original[Derch];
                     fDoIzq=Original[DoIzq]; fDoCen=Original[DoCen]; fDoDer=Original[DoDer];
                     Divisor=6.0;
                  }
                  else 
                  {
                     if (i > PriUltFil) /* elementos intermedios ultima fila */
                     {
                        fUpIzq=Original[UpIzq]; fUpCen=Original[UpCen]; fUpDer=Original[UpDer];
                        fIzqda=Original[Izqda]; fCentr=Original[Centr]; fDerch=Original[Derch];
                        fDoIzq=.0;            fDoCen=.0;            fDoDer=.0;
                        Divisor=6.0;
                     }
                     else 
                     {
                        if (PriCol == 0) /* elementos primera columna no tratados ya */
                        {
                           fUpIzq=.0; fUpCen=Original[UpCen]; fUpDer=Original[UpDer];
                           fIzqda=.0; fCentr=Original[Centr]; fDerch=Original[Derch];
                           fDoIzq=.0; fDoCen=Original[DoCen]; fDoDer=Original[DoDer];
                           Divisor=6.0;
                        }
                        else
                        {
                           if (UltCol == 0) /* elementos ultima columnas no tratados ya */
                           {
                              fUpIzq=Original[UpIzq]; fUpCen=Original[UpCen]; fUpDer=.0;
                              fIzqda=Original[Izqda]; fCentr=Original[Centr]; fDerch=.0;
                              fDoIzq=Original[DoIzq]; fDoCen=Original[DoCen]; fDoDer=.0;
                              Divisor=6.0;
                           }
                           else /* caso general */
                           {
                              fUpIzq=Original[UpIzq]; fUpCen=Original[UpCen]; fUpDer=Original[UpDer];
                              fIzqda=Original[Izqda]; fCentr=Original[Centr]; fDerch=Original[Derch];
                              fDoIzq=Original[DoIzq]; fDoCen=Original[DoCen]; fDoDer=Original[DoDer];
                              Divisor=9.0;
			
                           }
                        }
                     }
                  }
               }
            }
         }     
      }
      

		Tratada[i] = (fUpIzq+fUpCen+fUpDer + fIzqda+fCentr+fDerch + fDoIzq+fDoCen+fDoDer) / Divisor;   
	
  } //Fin del if de comprobacion
}





int main(int argc, char* argv[]) {
   unsigned char *Cabecera=NULL;
   char          *Entrada=NULL;
   char          *Salida=NULL;

   unsigned int   nDimX, nDimY, CabeceraSize, rc, Itera, i;
   unsigned short BytePorPixel;

   int HilosPorBloque=0, BloquesPorGrid=0;

   float *ImagenTratada=NULL,    *ImagenOriginal=NULL;
   float *GPUTratada=NULL, *GPUOriginal=NULL;
   float time=.0; 

   cudaEvent_t start, stop;

   if (argc != 5) {
      printf("Uso: Suaviza-Secuencial <Fichero-Entrada> <Fichero-Salida> <Iteraciones> <Threads per Block>\n");
      return -1;
   }

   Entrada        = strdup(argv[1]);
   Salida         = strdup(argv[2]);
   Itera          = atoi(argv[3]);
   HilosPorBloque = atoi(argv[4]);
   
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

   for (i=1; i<=Itera; i++)
   {
      if ((i % 2) != 0)
         SuavizaCuda<<<BloquesPorGrid, HilosPorBloque>>>(GPUTratada,  GPUOriginal,  nDimX, nDimY);
      else
         SuavizaCuda<<<BloquesPorGrid, HilosPorBloque>>>(GPUOriginal, GPUTratada, nDimX, nDimY);
   }
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
