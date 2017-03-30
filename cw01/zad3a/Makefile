LIBRARY_NAME = addressBook
SOURCE_FILES = listAddressBook.c treeAddressBook.c
OBJECT_FILES = $(SOURCE_FILES:.c=.o)

all: compile staticlib sharedlib static shared dynamic

static:
	gcc -std=c99 -o main_stat main.c libaddressBook.a
	echo Static > results.txt
	./main_stat >> results.txt

shared:
	gcc -std=c99 -o main_shared main.c -Wl,-rpath=. -L. -laddressBook
	echo Shared >> results.txt
	./main_shared >> results.txt

dynamic:
	gcc -std=c99 main_dyn.c -o main_dynamic -ldl
	echo Dynamic >> results.txt
	./main_dynamic >> results.txt


sharedlib: lib$(LIBRARY_NAME).so

lib$(LIBRARY_NAME).so: $(SOURCE_FILES)
	gcc -std=gnu11 -Wall -Wpedantic -fPIC -shared $^ -o $@
	
staticlib: lib$(LIBRARY_NAME).a

lib$(LIBRARY_NAME).a: $(OBJECT_FILES)
	ar rcs $@ $^
	
compile: $(OBJECT_FILES)

$(OBJECT_FILES): $(SOURCE_FILES)
	gcc -c -std=gnu11 -Wall -Wpedantic $^

clean:
	find . -maxdepth 1 \( -name "*.[ao]" -o -name "*.so" \) -exec rm {} \;

.PHONE: all sharedlib staticlib static shared dynamic clean
