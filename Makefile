COMMON_FLAGS=$(shell python-config --includes) -I. -fPIC -O3 -g
CC=gcc

all: ./remote_debug.so
	@echo compiling done

clean:
	find . -name \*.lo -o -name \*.o | xargs rm -f
	find . -name \*.la -o -name \*.a | xargs rm -f 
	find . -name \*.so | xargs rm -f 

./remote_debug.lo: remote_debug.c
	$(CC) $(COMMON_FLAGS) -c remote_debug.c -o remote_debug.lo 

remote_debug.so: remote_debug.lo
	$(CC) $(COMMON_FLAGS) -shared remote_debug.lo -o remote_debug.so -lpython
