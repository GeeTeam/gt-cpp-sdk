

CC = g++
INCLUDES = 
LIBS =
CFLAGS =-Wall -Werror -g


TARGET = test
OBJS =  main.o \
		json/json_reader.o \
		json/json_writer.o \
		json/json_value.o 



all:  $(TARGET)

test:$(OBJS)
	$(CC)  $^ -o $@  $(LIBS)

%.o:%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f json/*.o
	rm -f *.o