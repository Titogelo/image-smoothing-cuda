int nGetSize(char *pszFileName,  unsigned int *pnDimX, unsigned int *pnDimY, 
	     unsigned short *BytePorPixel, unsigned int *CabeceraSize) {
        FILE           *fp;
        char           type[2];
        unsigned int   nVals[4];
        unsigned short TMP;
                
        fp = fopen(pszFileName, "rb");
        assert(fp);

	/* Leer los caracteres identificativos del bitmap, 2 bytes */
        fread(type, 2, 1, fp);
        if ((type[0] != 'B') || (type[1] != 'M')) {
           printf("Formato no adecuado\n");
 	   return -1;
        }

	/* Leer el tamano en bytes del fichero: 1 dword */
        fread(nVals, 1, 4, fp);

        /* Saltar el siguiente dword que es reservado para usos futuros */
        fread(nVals, 1, 4, fp);

        /* Leer 1 dword, distancia desde el inicio del fichero al area de datos */
        fread(CabeceraSize, 1, 4, fp);

	/* Leer 1 dword: Tamaño de la BitmapInfoHeader. En windows = 40 */
        fread(nVals, 1, 4, fp);
	if (nVals[0] != 40){
	  printf("Longitud del BitmapInfoHeader (%d != 40Bytes) Erronea\n", nVals[0]);
	  return -1;
	}

        /* Leer 2 dwords, Dimensiones Horizontal (X) y Vertical (Y) */
        fread(pnDimX, 1, 4, fp);
        fread(pnDimY, 1, 4, fp);
  
        /* Leer 1 word que es el numero de planos del bitmap */
        fread(&TMP, 1, 2, fp);

        /* Leer 1 word, que es el numero de bits por pixel        */
	/* Puede ser 1, 4, 8, 16, 24, 32. No se trabaja con 1 y 4 */
        fread(&TMP, 1, 2, fp);
        if ((TMP != 8) && (TMP != 16) && (TMP != 24) && (TMP != 32)) {
           printf("No es BMP de 256, 16bit, 24bit o 32bit colores\n");
           return -1;
        }                     
        *BytePorPixel=TMP/8;

        /* Leer el tipo de compresion, 1 dword */
        fread(nVals, 1, 4, fp);
        if (nVals[0] != 0) {
           printf("No se trabaja con bitmap comprimidos\n");
           return -1;
        }                     

        /* Leer el numero de bytes de datos, 1 dword */
        fread(nVals, 1, 4, fp);

        /* Leer los 4 restantes dword que forman la cabecera */
        fread(nVals, 4, 4, fp);

        fclose(fp);
        return 0;
}

int nLoad(char *pszFileName, unsigned int nDimX, unsigned int nDimY, float **pucImage, unsigned char *Cabecera, unsigned int CabeceraSize)
{
  FILE *fp;
  unsigned char caracter;
  unsigned int i,j;  
  
  fp = fopen(pszFileName, "rb");
  assert(fp);
  
  /* Leer Cabecera */
  fread(Cabecera, CabeceraSize, 1, fp);

  /* Leer Datos */
  for (i=0; i<nDimY; i++)
  {
     for (j=0; j<nDimX; j++)
     {
        fread(&caracter, 1, 1, fp);
        pucImage[i][j]=(float)caracter;
      }
  }
  fclose(fp);
  return 0;
}


int nLoad2(char *pszFileName, unsigned int nDimX, unsigned int nDimY, float **pucImage, unsigned char *Cabecera, unsigned int CabeceraSize)
{
  FILE *fp;
  unsigned char caracter;
  unsigned int i,j;  
  
  fp = fopen(pszFileName, "rb");
  assert(fp);
  
  /* Leer Cabecera */
  fread(Cabecera, CabeceraSize, 1, fp);

  /* Leer Datos */
  for (i=1; i<=nDimY; i++)
  {
     for (j=1; j<=nDimX; j++)
     {
        fread(&caracter, 1, 1, fp);
        pucImage[i][j]=(float)caracter;
      }
  }
  fclose(fp);
  
  /* Regularizando fronteras. Una forma */
  for (j=1; j<=nDimX; j++)
  {
           pucImage[0][j]=pucImage[1][j];
     pucImage[nDimY+1][j]=pucImage[nDimY][j];
  }  

  for (j=1; j<=nDimY; j++)
  {
           pucImage[j][0]=pucImage[j][1];
     pucImage[j][nDimX+1]=pucImage[j][nDimX];
  }  
    
  pucImage[0][0]             = pucImage[1][1];
  pucImage[0][nDimX+1]       = pucImage[1][nDimX];
  pucImage[nDimY+1][0]       = pucImage[nDimY][1];
  pucImage[nDimY+1][nDimX+1] = pucImage[nDimY][nDimX];
  
  return 0;
}

int nLoad3(char *pszFileName, unsigned int nDimX, unsigned int nDimY, float *pucImage, unsigned char *Cabecera, unsigned int CabeceraSize)
{
  FILE *fp;
  unsigned char caracter;
  unsigned int i;  
  
  fp = fopen(pszFileName, "rb");
  assert(fp);
  
  /* Leer Cabecera */
  fread(Cabecera, CabeceraSize, 1, fp);

  /* Leer Datos */
  for (i=0; i<nDimY*nDimX; i++)
  {
     fread(&caracter, 1, 1, fp);
     pucImage[i]=(float)caracter;
  }
  fclose(fp);
  return 0;
}


int nSave(char *pszFileName, unsigned int nDimX, unsigned int nDimY, float **pucImage, unsigned char *Cabecera, unsigned int CabeceraSize)
{
  FILE *fp;

  unsigned char tmpchar;
  unsigned int  i,j;
  float         tmpfloat;

  fp = fopen(pszFileName, "wb");
  assert(fp);
  
  /* Escribir Cabecera */
  fwrite(Cabecera, CabeceraSize, 1, fp);
  
  /* Escribir datos */
  for (i=0; i<nDimY; i++)
  {
     for (j=0; j<nDimX; j++)
     {
        tmpfloat=pucImage[i][j];
        if (tmpfloat <= .0){
           tmpchar=0;
        }
        else {
           if (tmpfloat >= 255) {
              tmpchar=255;
           }
           else
              tmpchar=(unsigned char)tmpfloat;
        }
        fwrite(&tmpchar, 1, 1, fp);
     }
  }
  fclose(fp);
  return 0;
}


int nSave2(char *pszFileName, unsigned int nDimX, unsigned int nDimY, float **pucImage, unsigned char *Cabecera, unsigned int CabeceraSize)
{
  FILE *fp;

  unsigned char tmpchar;
  unsigned int  i,j;
  float         tmpfloat;

  fp = fopen(pszFileName, "wb");
  assert(fp);
  
  /* Escribir Cabecera */
  fwrite(Cabecera, CabeceraSize, 1, fp);
  
  /* Escribir datos */
  /* Leer Datos */
  for (i=1; i<=nDimY; i++)
  {
     for (j=1; j<=nDimX; j++)
     {
        tmpfloat=pucImage[i][j];
        if (tmpfloat <= .0){
           tmpchar=0;
        }
        else {
           if (tmpfloat >= 255) {
              tmpchar=255;
           }
           else
              tmpchar=(unsigned char)tmpfloat;
        }
        fwrite(&tmpchar, 1, 1, fp);
     }
  }
  fclose(fp);
  return 0;
}

int nSave3(char *pszFileName, unsigned int nDimX, unsigned int nDimY, float *pucImage, unsigned char *Cabecera, unsigned int CabeceraSize)
{
  FILE *fp;

  unsigned char tmpchar;
  unsigned int  i;
  float         tmpfloat;

  fp = fopen(pszFileName, "wb");
  assert(fp);
  
  /* Escribir Cabecera */
  fwrite(Cabecera, CabeceraSize, 1, fp);
  
  /* Escribir datos */
  for (i=0; i<nDimY*nDimX; i++)
  {
     tmpfloat=pucImage[i];
     if (tmpfloat <= .0){
        tmpchar=0;
     }
     else {
        if (tmpfloat >= 255) {
           tmpchar=255;
        }
        else
           tmpchar=(unsigned char)tmpfloat;
     }
     fwrite(&tmpchar, 1, 1, fp);
  }
  fclose(fp);
  return 0;
}





float **Crea_Matriz(int nDimY, int nDimX) {
  float ** p;
  p = (float **)calloc(nDimY, sizeof(float *));
  while (nDimY--)
     p[nDimY] = (float *)calloc(nDimX, sizeof(float));
  return p;
}
          
void Liberar_Matriz(float **matriz, int nDimY) {
  int i;
  if (matriz != NULL) {
     for (i=0; i<nDimY; i++) {
       free(matriz[i]);
       matriz[i]=NULL;
     }
     free(matriz);
     matriz = NULL;
  }
}


