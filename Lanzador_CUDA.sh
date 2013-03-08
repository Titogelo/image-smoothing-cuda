#!/bin/sh

# Para decirle a SGE que sh es el shell del job
#$ -S /bin/sh

#Primero Compilamos
make

#comprobamos que la compilacion fue bien. Esto es,
#que existe el fichero ejecutable ejemplo_secuencial
if [ ! -x Suma ]; then
   echo "Upps, la compilacion no fue bien"
   exit 1
fi

if [ ! -x TipoCard ]; then
   echo "Upps, la compilacion no fue bien"
   exit 1
fi


#Ahora lanzamos la ejecucion en SGE del ejecutable
./TipoCard
./Suma  100
