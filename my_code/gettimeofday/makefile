cc = arm-linux-gnueabihf-gcc
#cc= g++
#cc= gcc
#cc= aarch64-linux-gnu-gcc
OBJ = gettimeofday

all :
	$(cc)  -c  ./*.c
	$(cc)  -o $(OBJ)  ./*.o -g
clean:
	rm -rf *.o
	rm -rf $(OBJ)