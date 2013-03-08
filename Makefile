FLAGS = -O2 -Wall

all : cleanall	Negativiza Suaviza Binariza Suaviza2 Suaviza3 Suaviza4 Suaviza5 Plantilla  PlantillaEmul
		@ echo "Compilado "

cleanall : clean
	@rm -f 	Negativiza Suaviza Binariza Suaviza2 Suaviza3 Suaviza4 Suaviza5 Plantilla  PlantillaEmul

clean :
	@rm -f *.o core *~


Negativiza : Negativiza.c
	gcc -o Negativiza Negativiza.c $(FLAGS)

Suaviza : Suaviza.c
	gcc -o Suaviza Suaviza.c $(FLAGS)

Suaviza2 : Suaviza2.c
	gcc -o Suaviza2 Suaviza2.c $(FLAGS)

Suaviza3 : Suaviza3.c
	gcc -o Suaviza3 Suaviza3.c $(FLAGS)

Suaviza4 : Suaviza4.c
	gcc -o Suaviza4 Suaviza4.c $(FLAGS)

Suaviza5 : Suaviza5.c
	gcc -o Suaviza5 Suaviza5.c $(FLAGS)

Binariza : Binariza.c
	gcc -o Binariza Binariza.c $(FLAGS)

Plantilla : Plantilla.cu
	nvcc --host-compilation C -o Plantilla Plantilla.cu -I/opt/cuda/SDK/C/common/inc/ 

PlantillaEmul : Plantilla.cu
	nvcc --host-compilation C -o PlantillaEmul Plantilla.cu -I/opt/cuda/SDK/C/common/inc/ --device-emulation 
	