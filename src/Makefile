
etherDump : dumper.o init.o main.o
	cc dumper.o init.o main.o -o etherdump

dumper.o : dumper.c dumper.h
init.o : init.c init.h 
main.o : main.c

clean:
	rm -f *.o *.gch *~ etherdump

