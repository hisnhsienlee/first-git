#cc = arm-linux-gnueabihf-gcc
cc= g++
#cc= gcc
#cc= aarch64-linux-gnu-gcc
OBJ = myshell

all : 
	$(cc)  -c  ./*.cpp -ldl -lpthread
	$(cc)  -c  ./sms/*.cpp -ldl -lpthread
	$(cc)  -o $(OBJ)  ./*.o -g -ldl -lpthread -lglib-2.0
clean:
	rm -rf *.o
	rm -rf $(OBJ)