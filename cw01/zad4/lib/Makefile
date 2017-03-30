LIBRARY_NAME = addressBook
SOURCE_FILES = listAddressBook.c treeAddressBook.c
OBJECT_FILES = $(SOURCE_FILES:.c=.o)

all: compile static shared clean

shared: lib$(LIBRARY_NAME).so

lib$(LIBRARY_NAME).so: $(SOURCE_FILES)
	gcc -std=gnu11 -Wall -Wpedantic -fPIC -shared $^ -o $@
	
static: lib$(LIBRARY_NAME).a

lib$(LIBRARY_NAME).a: $(OBJECT_FILES)
	ar rcs $@ $^
	
compile: $(OBJECT_FILES)

$(OBJECT_FILES): $(SOURCE_FILES)
	gcc -c -std=gnu11 -Wall -Wpedantic $^

clean:
	find . -maxdepth 1 \( -name "*.[ao]" -o -name "*.so" \) -exec rm {} \;

.PHONY: all shared static compile clean
