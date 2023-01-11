CC	=	gcc
CFLAGS	=	-Iinclude
exec 	= 	webserver.o
sources = 	$(wildcard src/*.c)
objects = 	$(sources:.c=.o)
flags 	= 	-g -Wall -lm -ldl -fPIC -rdynamic 

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c %.h
	gcc -c $(flags) $< -o $@


clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.a
	-rm src/*.o=
