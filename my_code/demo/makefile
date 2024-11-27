#cc = arm-linux-gnueabihf-gcc
#cc=arm-linux-gnueabihf-g++
#cc= g++
#cc= gcc
cc= aarch64-linux-gnu-g++
OBJ = demo

all : 
	$(cc) -std=c++11 -c -g  ./*.c -pthread -lwnc -L ./
	$(cc) -std=c++11 -g -o	 $(OBJ)  ./*.o -pthread -lwnc -L ./
clean:
	rm -rf *.o
	rm -rf $(OBJ)
