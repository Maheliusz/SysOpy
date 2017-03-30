LIBRARY_NAME = addressBook
SOURCE_FILES = listAddressBook.c treeAddressBook.c
OBJECT_FILES = $(SOURCE_FILES:.c=.o)

all: compile staticlib sharedlib staticoptimizers sharedoptimizers dynamicoptimizers

sharedlib: lib$(LIBRARY_NAME).so

lib$(LIBRARY_NAME).so: $(SOURCE_FILES)
	gcc -std=gnu11 -Wall -Wpedantic -fPIC -shared $^ -o $@
	
staticlib: lib$(LIBRARY_NAME).a

lib$(LIBRARY_NAME).a: $(OBJECT_FILES)
	ar rcs $@ $^
	
compile: $(OBJECT_FILES)

$(OBJECT_FILES): $(SOURCE_FILES)
	gcc -c -std=gnu11 -Wall -Wpedantic $^

staticoptimizers:
	gcc -std=c99 -O0 -o main_stat main.c libaddressBook.a
	echo Static_opt0 > results.txt
	./main_stat >> results.txt
	gcc -std=c99 -O1 -o main_stat main.c libaddressBook.a
	echo Static_opt1 >> results.txt
	./main_stat >> results.txt
	gcc -std=c99 -O2 -o main_stat main.c libaddressBook.a
	echo Static_opt2 >> results.txt
	./main_stat >> results.txt
	gcc -std=c99 -O3 -o main_stat main.c libaddressBook.a
	echo Static_opt3 >> results.txt
	./main_stat >> results.txt
	gcc -std=c99 -Os -o main_stat main.c libaddressBook.a
	echo Static_opts >> results.txt
	./main_stat >> results.txt
	gcc -std=c99 -Ofast -o main_stat main.c libaddressBook.a
	echo Static_optfast >> results.txt
	./main_stat >> results.txt
	gcc -std=c99 -Og -o main_stat main.c libaddressBook.a
	echo Static_optg >> results.txt
	./main_stat >> results.txt

sharedoptimizers:
	gcc -std=c99 -O0 -o main_shared main.c -Wl,-rpath=. -L. -laddressBook
	echo Shared_opt0 >> results.txt
	./main_shared >> results.txt
	gcc -std=c99 -O1 -o main_shared main.c -Wl,-rpath=. -L. -laddressBook
	echo Shared_opt1 >> results.txt
	./main_shared >> results.txt
	gcc -std=c99 -O2 -o main_shared main.c -Wl,-rpath=. -L. -laddressBook
	echo Shared_opt2 >> results.txt
	./main_shared >> results.txt
	gcc -std=c99 -O3 -o main_shared main.c -Wl,-rpath=. -L. -laddressBook
	echo Shared_opt3 >> results.txt
	./main_shared >> results.txt
	gcc -std=c99 -Os -o main_shared main.c -Wl,-rpath=. -L. -laddressBook
	echo Shared_opts >> results.txt
	./main_shared >> results.txt
	gcc -std=c99 -Ofast -o main_shared main.c -Wl,-rpath=. -L. -laddressBook
	echo Shared_optfast >> results.txt
	./main_shared >> results.txt
	gcc -std=c99 -Og -o main_shared main.c -Wl,-rpath=. -L. -laddressBook
	echo Shared_optg >> results.txt
	./main_shared >> results.txt

dynamicoptimizers:
	gcc -std=c99 -O0 -o main_dynamic main_dyn.c -ldl
	echo Dynamic_opt0 >> results.txt
	./main_dynamic >> results.txt
	gcc -std=c99 -O1 -o main_dynamic main_dyn.c -ldl
	echo Dynamic_opt1 >> results.txt
	./main_dynamic >> results.txt
	gcc -std=c99 -O2 -o main_dynamic main_dyn.c -ldl
	echo Dynamic_opt2 >> results.txt
	./main_dynamic >> results.txt
	gcc -std=c99 -O3 -o main_dynamic main_dyn.c -ldl
	echo Dynamic_opt3 >> results.txt
	./main_dynamic >> results.txt
	gcc -std=c99 -Os -o main_dynamic main_dyn.c -ldl
	echo Dynamic_opts >> results.txt
	./main_dynamic >> results.txt
	gcc -std=c99 -Ofast -o main_dynamic main_dyn.c -ldl
	echo Dynamic_optfast >> results.txt
	./main_dynamic >> results.txt
	gcc -std=c99 -Og -o main_dynamic main_dyn.c -ldl
	echo Dynamic_optg >> results.txt
	./main_dynamic >> results.txt

clean:
	find . -maxdepth 1 \( -name "*.[ao]" -o -name "*.so" \) -exec rm {} \;

.PHONE: all sharedlib staticlib compile staticoptimizers sharedoptimizers dynamicoptimizers clean
