CC=gcc
OBJ=pthread_create.c
obj=pthread_create
target=pthread_create
CFLAGS=-Wall -g -lpthread

$(target):$(obj).o
	$(CC) -o $@ $^ $(CFLAGS)

$(obj).o:$(obj).s
	$(CC) -c $< -o $@
$(obj).s:$(obj).i
	$(CC) -S $< -o $@
$(obj).i:$(OBJ)
	$(CC) -E $< -o $@

clean:
	rm -rf ./*.i ./*.s ./*.o
