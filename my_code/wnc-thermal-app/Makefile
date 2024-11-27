CC = aarch64-linux-gnu-gcc
TARGET = wnc-thermal-app
OBJS = wnc-thermal-app.o

CFLAGS += -Wno-undef -Werror -Os -funit-at-a-time -pipe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) -lpthread

clean:
	rm -f *.o $(TARGET)
