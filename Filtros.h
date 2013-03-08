void Negativiza(float **Imagen, unsigned int nDimX, unsigned int nDimY) {
   unsigned int i, j;

   for (i=0; i<nDimY; i++)
     for (j=0; j<nDimX; j++)
       Imagen[i][j]=255-Imagen[i][j];
}

void Binariza(float **Imagen, unsigned int nDimX, unsigned int nDimY, float Media)
{
   unsigned int i,j;

   for (i=0; i<nDimY; i++)
     for (j=0; j<nDimX; j++)
     
       if (Imagen[i][j]<=Media)
         Imagen[i][j]=255;
       else
         Imagen[i][j]=0;
}


void Suaviza(float **Imagen, float *tImagen, float *aImagen, unsigned int nDimX, unsigned int nDimY)
{
   unsigned int i, j;

   /* Copiando la fila actual en la iteracion (t) al area temporal tImage */
   bcopy(&Imagen[0][0], tImagen, nDimX*sizeof(float));

   /* Primera Fila: Primer elemento */
   Imagen[0][0] = (Imagen[0][0]+Imagen[0][1]+Imagen[1][0]+Imagen[1][1]) / 4;

   /* Primera Fila: Elementos intermedios */
   for (j=1; j<(nDimX-1); j++) 
   {
     Imagen[0][j] = (tImagen[j-1]+Imagen[0][j]+Imagen[0][j+1] + 
                   Imagen[1][j-1]+Imagen[1][j]+Imagen[1][j+1]) / 6;
   }
       
   /* Primera Fila: Ultimo elemento */  
   Imagen[0][nDimX-1] = (tImagen[nDimX-2]+Imagen[0][nDimX-1] +
                       Imagen[1][nDimX-2]+Imagen[1][nDimX-1]) / 4; 

   /* Filas Intermedias */
   for (i=1; i<(nDimY-1); i++)
   {
     /* Copiando el area temporal tImage al area temporal aImage (arriba) */
     bcopy(tImagen, aImagen, nDimX*sizeof(float));

     /* Copiando la fila actual en la iteracion (t) al area temporal tImage*/
     bcopy(&Imagen[i][0], tImagen, nDimX*sizeof(float));

     /* Primer elemento de la columna */
     Imagen[i][0] = (aImagen[0] + aImagen[1] +
                   Imagen[i][0]+Imagen[i][1] + Imagen[i+1][0]+Imagen[i+1][1]) / 6;
       
     /* Elementos intermedios de la columna */
     for (j=1; j<(nDimX-1); j++)
     {
       Imagen[i][j]=(aImagen[j-1] + aImagen[j] + aImagen[j+1] +
                     tImagen[j-1]+Imagen[i][j]+Imagen[i][j+1] +
                 Imagen[i+1][j-1]+Imagen[i+1][j]+Imagen[i+1][j+1]) / 9;
     }
       
     /* Ultimo elemento de la Columna */
     Imagen[i][nDimX-1]=(aImagen[nDimX-2] + aImagen[nDimX-1] + 
                         tImagen[nDimX-2]+Imagen[i][nDimX-1] + 
                     Imagen[i+1][nDimX-2]+Imagen[i+1][nDimX-1]) / 6;
   }
     
   /* Ultima Fila */
   /* Copiando el area temporal tImage al area temporal aImage (arriba) */
   bcopy(tImagen, aImagen, nDimX*sizeof(float));

   /* Copiando la fila actual en la iteracion (t) al area temporal tImage*/
   bcopy(&Imagen[nDimY-1][0], tImagen, nDimX*sizeof(float));

   /* Primer elemento de la columna */
   Imagen[nDimY-1][0]=(aImagen[0]+aImagen[1]+Imagen[nDimY-1][0]+Imagen[nDimY-1][1]) / 4;

   /* Elementos intermedios de la columna */
   for (j=1; j<(nDimX-1); j++)
   {
     Imagen[nDimY-1][j]=(aImagen[j-1]+aImagen[j]+aImagen[j+1] +
                         tImagen[j-1]+Imagen[nDimY-1][j]+Imagen[nDimY-1][j+1]) / 6;
   }
       
   /* Ultimo elemento de la Columna */
   Imagen[nDimY-1][nDimX-1]=(aImagen[nDimX-2]+aImagen[nDimX-1] +
                             tImagen[nDimX-2]+Imagen[nDimY-1][nDimX-1]) / 4;
}


void Suaviza2(float **Imagen, float *tImagen, float *aImagen, unsigned int nDimX, unsigned int nDimY)
{
   unsigned int i, j;

   /* Copiando la fila actual en la iteracion (t) al area temporal tImage */
   bcopy(&Imagen[0][0], tImagen, (nDimX+2)*sizeof(float));

   for (i=1; i<=nDimY; i++)
   {
     /* Copiando el area temporal tImage al area temporal aImage (arriba) */
     bcopy(tImagen, aImagen, (nDimX+2)*sizeof(float));

     /* Copiando la fila actual en la iteracion (t) al area temporal tImage*/
     bcopy(&Imagen[i][0], tImagen, (nDimX+2)*sizeof(float));

     for (j=1; j<=nDimX; j++)
       Imagen[i][j]=(aImagen[j-1] + aImagen[j] + aImagen[j+1] +
                     tImagen[j-1]+Imagen[i][j]+Imagen[i][j+1] +
                 Imagen[i+1][j-1]+Imagen[i+1][j]+Imagen[i+1][j+1]) / 9;
   }
}


void Suaviza1D(float *Imagen, float *tImage, float *aImage, unsigned int nDimX, unsigned int nDimY)
{
   unsigned int TMP, i, j, auxi1, auxi2;

   /* Copiando la fila actual en la iteracion (t) al area temporal tImage */
   bcopy(&Imagen[0], tImage, nDimX*sizeof(float));

   /* Primera Fila: Primer elemento */
   Imagen[0]=(Imagen[0]+Imagen[1]+Imagen[nDimX]+Imagen[nDimX+1]) / 4.0;

   /* Primera Fila: Elementos intermedios */
   for (j=1; j<(nDimX-1); j++) {
     auxi1 = j+nDimX;     
     Imagen[j]=(tImage[j-1]+Imagen[j]+Imagen[j+1]+Imagen[auxi1-1]+Imagen[auxi1]+Imagen[auxi1+1]) / 6.0;
   }
       
   /* Primera Fila: Ultimo elemento */  
   Imagen[nDimX-1]=(tImage[nDimX-2]+Imagen[nDimX-1]+Imagen[2*nDimX-2]+Imagen[2*nDimX-1]) / 4.0; 
     
   /* Filas Intermedias */
   for (i=1; i<(nDimY-1); i++) {
     TMP=i*nDimX;

     /* Copiando el area temporal tImage al area temporal aImage (arriba) */
     bcopy(tImage, aImage, nDimX*sizeof(float));

     /* Copiando la fila actual en la iteracion (t) al area temporal tImage*/
     bcopy(&Imagen[TMP], tImage, nDimX*sizeof(float));

     /* Primer elemento de la columna */
     Imagen[TMP]=(aImage[0]+aImage[1]+Imagen[TMP]+Imagen[TMP+1]+Imagen[TMP+nDimX]+Imagen[TMP+nDimX+1]) / 6.0;
       
     /* Elementos intermedios de la columna */
     for (j=1; j<(nDimX-1); j++) {
       auxi1 = TMP + j;
       auxi2 = auxi1 + nDimX;

       Imagen[auxi1]=(aImage[j-1]+aImage[j]+aImage[j+1] + tImage[j-1]+Imagen[auxi1]+Imagen[auxi1+1]+
		      Imagen[auxi2-1]+Imagen[auxi2]+Imagen[auxi2+1]) / 9.0;
     }
       
     /* Ultimo elemento de la Columna */
     auxi1 = TMP + nDimX - 1;
     auxi2 = TMP + 2*nDimX;
     Imagen[auxi1]=(aImage[nDimX-2]+aImage[nDimX-1]+tImage[nDimX-2]+Imagen[auxi1]+Imagen[auxi2-2]+Imagen[auxi2-1]) / 6.0;
   }
     
   /* Ultima Fila */
   /* Copiando el area temporal tImage al area temporal aImage (arriba) */
   bcopy(tImage, aImage, nDimX*sizeof(float));

   /* Copiando la fila actual en la iteracion (t) al area temporal tImage*/
   bcopy(&Imagen[(nDimY-1)*nDimX], tImage, nDimX*sizeof(float));

   /* Primer elemento de la columna */
   auxi1 = (nDimY-1)*nDimX;
   Imagen[auxi1]=(aImage[0]+aImage[1]+Imagen[auxi1]+Imagen[auxi1+1]) / 4.0;
      
   /* Elementos intermedios de la columna */
   for (j=1; j<(nDimX-1); j++) {
     auxi1 = (nDimY-1)*nDimX + j;
     Imagen[auxi1]=(aImage[j-1]+aImage[j]+aImage[j+1]+tImage[j-1]+Imagen[auxi1]+Imagen[auxi1+1]) / 6.0;
   }
       
   /* Ultimo elemento de la Columna */
   auxi1 = nDimY*nDimX-1;
   Imagen[auxi1]=(aImage[nDimX-2]+aImage[nDimX-1]+tImage[nDimX-2]+Imagen[auxi1]) / 4.0;
}



void Suaviza1D2(float *Updated, float *Backup, unsigned int nDimX, unsigned int nDimY)
{
   unsigned int TMP, i, j, auxi1, auxi2, auxi3;

   /* Primera Fila: Primer elemento */
   Updated[0]=(Backup[0]+Backup[1]+Backup[nDimX]+Backup[nDimX+1]) / 4.0;

   /* Primera Fila: Elementos intermedios */
   for (j=1; j<(nDimX-1); j++)
   {
      TMP = j+nDimX;     
      Updated[j]=(Backup[j-1]+Backup[j]+Backup[j+1]+Backup[TMP-1]+Backup[TMP]+Backup[TMP+1]) / 6.0;
   }
       
   /* Primera Fila: Ultimo elemento */  
   Updated[nDimX-1]=(Backup[nDimX-2]+Backup[nDimX-1]+Backup[2*nDimX-2]+Backup[2*nDimX-1]) / 4.0; 
     
   /* Filas Intermedias */
   for (i=1; i<(nDimY-1); i++)
   {
      TMP   = i*nDimX;
      auxi1 = TMP - nDimX;
      auxi2 = TMP + nDimX;

      /* Primer elemento de la columna */
      Updated[TMP]=(Backup[auxi1]+Backup[auxi1+1]+Backup[TMP]+Backup[TMP+1]+Backup[auxi2]+Backup[auxi2+1]) / 6.0;
       
      /* Elementos intermedios de la columna */
      for (j=1; j<(nDimX-1); j++)
      {
         auxi1 =   TMP + j;
         auxi2 = auxi1 - nDimX;
         auxi3 = auxi1 + nDimX;

         Updated[auxi1]=(Backup[auxi2-1]+Backup[auxi2]+Backup[auxi2+1] + Backup[auxi1-1]+Backup[auxi1]+Backup[auxi1+1]+
	  	         Backup[auxi3-1]+Backup[auxi3]+Backup[auxi3+1]) / 9.0;
      }
       
      /* Ultimo elemento de la Columna */
      auxi1 =   TMP + nDimX - 1;
      auxi2 = auxi1 - nDimX;
      auxi3 = auxi1 + nDimX;
     
      Updated[auxi1]=(Backup[auxi2-1]+Backup[auxi2]+Backup[auxi1-1]+Backup[auxi1]+Backup[auxi3-1]+Backup[auxi3]) / 6.0;
   }
     
   /* Ultima Fila */
   TMP = (nDimY-1)*nDimX;
   
   /* Primer elemento de la columna */
   auxi1 = TMP;
   auxi2 = TMP - nDimX;
   
   Updated[auxi1]=(Backup[auxi2]+Backup[auxi2+1]+Backup[auxi1]+Backup[auxi1+1]) / 4.0;
      
   /* Elementos intermedios de la columna */
   for (j=1; j<(nDimX-1); j++)
   {
      auxi1 = TMP + j;
      auxi2 = auxi1 - nDimX;
      Updated[auxi1]=(Backup[auxi2-1]+Backup[auxi2]+Backup[auxi2+1]+Backup[auxi1-1]+Backup[auxi1]+Backup[auxi1+1]) / 6.0;
   }
       
   /* Ultimo elemento de la Columna */
   auxi1 = nDimY*nDimX-1;
   auxi2 = auxi1 - nDimX;
   Updated[auxi1]=(Backup[auxi2-1]+Backup[auxi2]+Backup[auxi1-1]+Backup[auxi1]) / 4.0;
}


void SuavizaCuda(float *Updated, float *Backup, unsigned int nDimX, unsigned int nDimY)
{
  unsigned int
      UpIzq, UpCen, UpDer,
      Izqda, Centr, Derch,
      DoIzq, DoCen, DoDer,
      UltPriFil, PriUltFil, UltUltFil,
      PriCol, UltCol, i;
   
   float
      fUpIzq, fUpCen, fUpDer,
      fIzqda, fCentr, fDerch,
      fDoIzq, fDoCen, fDoDer,
      Divisor;

   UltPriFil=nDimX-1;
   PriUltFil=(nDimY-1)*nDimX;
   UltUltFil=nDimY*nDimX-1;
   
   
   for (i=0; i<nDimY*nDimX; i++)
   {
      UpCen=i-nDimX; UpIzq=UpCen-1; UpDer=UpCen+1;
      Centr=i;       Izqda=Centr-1; Derch=Centr+1;
      DoCen=i+nDimX; DoIzq=DoCen-1; DoDer=DoCen+1;

      PriCol=i     % nDimX;
      UltCol=(i+1) % nDimX;
   
      if (i == 0) /* elemento (0,0) */
      {
         fUpIzq=.0; fUpCen=.0;            fUpDer=.0;
         fIzqda=.0; fCentr=Backup[Centr]; fDerch=Backup[Derch];
         fDoIzq=.0; fDoCen=Backup[DoCen]; fDoDer=Backup[DoDer];
         Divisor=4.0;
      }
      else
      {
         if (i == UltPriFil) /* elemento (0, nDimX-1) */
         {
            fUpIzq=.0;            fUpCen=.0;            fUpDer=.0;
            fIzqda=Backup[Izqda]; fCentr=Backup[Centr]; fDerch=.0;
            fDoIzq=Backup[DoIzq]; fDoCen=Backup[DoCen]; fDoDer=.0;
            Divisor=4.0;
         }
         else
         {
            if (i == PriUltFil) /* elemento (nDimY-1, 0) */
            {
               fUpIzq=.0; fUpCen=Backup[UpCen]; fUpDer=Backup[UpDer];
               fIzqda=.0; fCentr=Backup[Centr]; fDerch=Backup[Derch];
               fDoIzq=.0; fDoCen=.0;            fDoDer=.0;
               Divisor=4.0;
            }
            else
            {
               if (i == UltUltFil) /* elemento (nDimY-1, nDimX-1) */ 
               {
                  fUpIzq=Backup[UpIzq]; fUpCen=Backup[UpCen]; fUpDer=.0;
                  fIzqda=Backup[Izqda]; fCentr=Backup[Centr]; fDerch=.0;
                  fDoIzq=.0;            fDoCen=.0;            fDoDer=.0;
                  Divisor=4.0;
               }
               else 
               {
                  if (i < UltPriFil) /* elementos intermedios primera fila */
                  {
                     fUpIzq=.0;            fUpCen=.0;            fUpDer=.0;
                     fIzqda=Backup[Izqda]; fCentr=Backup[Centr]; fDerch=Backup[Derch];
                     fDoIzq=Backup[DoIzq]; fDoCen=Backup[DoCen]; fDoDer=Backup[DoDer];
                     Divisor=6.0;
                  }
                  else 
                  {
                     if (i > PriUltFil) /* elementos intermedios ultima fila */
                     {
                        fUpIzq=Backup[UpIzq]; fUpCen=Backup[UpCen]; fUpDer=Backup[UpDer];
                        fIzqda=Backup[Izqda]; fCentr=Backup[Centr]; fDerch=Backup[Derch];
                        fDoIzq=.0;            fDoCen=.0;            fDoDer=.0;
                        Divisor=6.0;
                     }
                     else 
                     {
                        if (PriCol == 0) /* elementos primera columna no tratados ya */
                        {
                           fUpIzq=.0; fUpCen=Backup[UpCen]; fUpDer=Backup[UpDer];
                           fIzqda=.0; fCentr=Backup[Centr]; fDerch=Backup[Derch];
                           fDoIzq=.0; fDoCen=Backup[DoCen]; fDoDer=Backup[DoDer];
                           Divisor=6.0;
                        }
                        else
                        {
                           if (UltCol == 0) /* elementos ultima columnas no tratados ya */
                           {
                              fUpIzq=Backup[UpIzq]; fUpCen=Backup[UpCen]; fUpDer=.0;
                              fIzqda=Backup[Izqda]; fCentr=Backup[Centr]; fDerch=.0;
                              fDoIzq=Backup[DoIzq]; fDoCen=Backup[DoCen]; fDoDer=.0;
                              Divisor=6.0;
                           }
                           else /* caso general */
                           {
                              fUpIzq=Backup[UpIzq]; fUpCen=Backup[UpCen]; fUpDer=Backup[UpDer];
                              fIzqda=Backup[Izqda]; fCentr=Backup[Centr]; fDerch=Backup[Derch];
                              fDoIzq=Backup[DoIzq]; fDoCen=Backup[DoCen]; fDoDer=Backup[DoDer];
                              Divisor=9.0;
                           }
                        }
                     }
                  }
               }
            }
         }     
      }
      Updated[i] = (fUpIzq+fUpCen+fUpDer + fIzqda+fCentr+fDerch + fDoIzq+fDoCen+fDoDer) / Divisor;
   }      
}


