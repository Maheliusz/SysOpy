all: libs static

static:
	gcc -std=c99 -Wall -g -o main main.c lib/libaddressBook.a

libs:
	make -C ./lib/

clean:
	find . -maxdepth 1 \( -name "*.[ao]" -o -name "*.so" \) -exec rm {} \;
	make clean -C ./lib/

.PHONE: all libs static clean
