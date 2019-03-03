

all: tests

utils.o:
	gcc -c ../utils.c -I.. -I../../common


test_utils: test_utils.c utils.o
	gcc -o test_utils test_utils.c utils.o

# Run all tests
tests: test_utils
	./test_utils

clean:
	rm -f *.o

mrproper: clean
	rm -f test_utils

