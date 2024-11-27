#cc = arm-linux-gnueabihf-g++
cc= g++
#cc= aarch64-linux-gnu-g++
OBJ = wnc_common
FLAGS = # -std=c++11
CFLAGS = -fPIC -g


all :
	$(cc)  -c  ./*.cpp $(FLAGS) $(CFLAGS)
	$(cc)  -o $(OBJ).so  ./*.o $(FLAGS) -shared $(CFLAGS)
clean:
	rm -rf *.o
	rm -rf $(OBJ).so