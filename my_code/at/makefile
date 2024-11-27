#cc = arm-linux-gnueabihf-gcc
#cc=arm-linux-gnueabihf-g++
#cc= g++
#cc= gcc
cc= aarch64-linux-gnu-g++
OBJ = atcmd

all : 
	$(cc) -std=c++11 -c -g  ./*.cpp -pthread -lrt
	$(cc) -std=c++11 -g -o	 $(OBJ)  ./*.o -pthread -lrt
clean:
	rm -rf *.o
	rm -rf $(OBJ)
