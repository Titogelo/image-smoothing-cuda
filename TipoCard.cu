/*---------------------------------------------------------------*/
/* example 02  - Device Management                               */
/* Description : get properties of all visible device/GPU        */
/* Version     : 1.0 for CUDA 2.0                                */
/* Compilation :                                                 */
/*   nvcc --host-compilation C -o example02 example02.cu         */
/* Execution   :                                                 */
/*   example02                                                   */
/*---------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <cuda.h>

int main (int argc, char *argv[] ) {
  int nbDevice, dev;
  struct cudaDeviceProp prop;

  cudaGetDeviceCount( &nbDevice );
  printf( "Number of device: %d\n", nbDevice );

  for( dev = 0 ; dev < nbDevice ; ++dev ) {
    printf( "\nProperties of CUDA device number %d\n", dev );
    cudaGetDeviceProperties( &prop, dev );

    printf( "  Device Name:        %s\n",     prop.name      );
    printf( "  Clock frequency:    %d kHz\n", prop.clockRate );
    printf( "  Compute capability: %d.%d\n",  prop.major, prop.minor );
    printf( "  Device Overlap:     " );
    switch( prop.deviceOverlap ) {
      case 0:
       printf( "No \n" );
      break;
      case 1:
       printf( "Yes\n" );
      break;
      default :
        printf( "Unknown (%d)\n", prop.deviceOverlap );
      break;
    }
    printf( "  Number of Multi-processor: %d\n", 
            prop.multiProcessorCount );
    printf( "  Global Memory Size:        %4d MB\n", 
            (prop.totalGlobalMem / 1048576) );
    printf( "  Memory Maximum Pitch:      %4d kB\n", 
            (prop.memPitch / 1024) );
    printf( "  Texture Alignment:         %4d bytes\n", 
            prop.textureAlignment );
    printf( "  Constant Memory Size:      %4d MB\n", 
            (prop.totalConstMem / 1024) );
    printf( "  Shared Memory Size :       %4d kB per multi-processor\n", 
            (prop.sharedMemPerBlock / 1024) );
    printf( "  Number of 32-bit registers: %d per multi-processor\n", 
            prop.regsPerBlock );
    printf( "  Grid maximum size (X,Y,Z):         %5d x %5d x %5d\n", 
            prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]  );
    printf( "  Thread bloc maximum size (X,Y,Z):  %5d x %5d x %5d\n", 
            prop.maxThreadsDim[0], 
            prop.maxThreadsDim[1],
            prop.maxThreadsDim[2]  );
    printf( "  Maximum Number of threads per block: %d per block of thread\n", 
            prop.maxThreadsPerBlock );
    printf( "  Warp Size: %d threads\n", prop.warpSize );
  }

  return 0;
}

