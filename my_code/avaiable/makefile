#cc = arm-linux-gnueabihf-gcc
#cc=arm-linux-gnueabihf-g++
#cc= g++
#cc= gcc
#cc= aarch64-linux-gnu-gcc
OBJ = myshell

all : 
	$(cc)  -std=c++11 -c  ./*.cpp -ldl -lpthread
	$(cc)  -std=c++11 -shared -fpic -o libtest.so test.cpp -lpthread -lglib-2.0
	$(cc)  -std=c++11 -o $(OBJ)  ./*.o -g -ldl -lpthread -lglib-2.0
clean:
	rm -rf *.o
	rm -rf $(OBJ)