#cc = arm-linux-gnueabihf-gcc
#cc=arm-linux-gnueabihf-g++
cc= g++
#cc= gcc
#cc= aarch64-linux-gnu-g++
OBJ = config

all : 
	$(cc)  -std=c++11 -c  ./*.cpp -pthread
	$(cc)  -std=c++11 -o $(OBJ)  ./*.o -g -pthread
clean:
	rm -rf *.o
	rm -rf $(OBJ)
