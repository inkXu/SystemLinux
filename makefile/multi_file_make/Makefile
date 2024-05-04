book:book.c sum.o mul.o
	gcc -o book book.c sum.o mul.o
sum.o:sum.c
	gcc -c sum.c -o sum.o
mul.o:mul.c
	gcc -c mul.c -o mul.o
clean:
	rm -rf *.o
