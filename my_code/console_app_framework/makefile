#CC = arm-linux-gnueabihf-g++
CC= g++
#CC= gcc
#cc= aarch64-linux-gnu-gcc
OBJ = Demo_App
DEFINE = -DRIL_SHLIB
all :
	$(CC) --std=c++11 -O3 -c ./*.cpp -ldl -lpthread -lstdc++ $(DEFINE)
	$(CC) --std=c++11 -O3 -o $(OBJ) ./*.o -g  -ldl -lpthread -lstdc++ $(DEFINE)
clean:
	rm -rf *.o
	rm -rf $(OBJ)
