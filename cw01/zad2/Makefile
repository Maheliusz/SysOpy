all: libs static shared clean

static:
	gcc -std=c99 -o main_stat main.c lib/libaddressBook.a

shared:
	gcc -std=c99 -o main_shared main.c -Wl,-rpath=./lib/ -L./lib/ -laddressBook

libs:
	make -C ./lib/

clean:
	find . -maxdepth 1 \( -name "*.[ao]" -o -name "*.so" \) -exec rm {} \;
	make clean -C ./lib/

.PHONE: all libs shared static clean
