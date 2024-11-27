#cc = arm-linux-gnueabihf-gcc
#cc=arm-linux-gnueabihf-g++
cc= g++
#cc= gcc
#cc= aarch64-linux-gnu-g++
OBJ = myshell

all : 
	$(cc) -std=c++11 -c -g  ./*.c -pthread
	$(cc) -std=c++11 -g -o	 $(OBJ)  ./*.o -pthread
clean:
	rm -rf *.o
	rm -rf $(OBJ)
