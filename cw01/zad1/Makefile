CC = gcc
CFLAGS = -std=gnu11 -Wall -Wpedantic

SOURCE_FILES = listAddressBook.c treeAddressBook.c
OBJECT_FILES = $(SOURCE_FILES:.c=.o)

all: shared static 

shared: lib$(LIBRARY_NAME).so

lib$(LIBRARY_NAME).so: $(SOURCE_FILES)
	$(CC) $(CFLAGS) -fPIC -shared $^ -o $@
	
static: lib$(LIBRARY_NAME).a

lib$(LIBRARY_NAME).a: $(OBJECT_FILES)
	ar rcs $@ $^
	
compile: $(OBJECT_FILES)

$(OBJECT_FILES): $(SOURCE_FILES)
	$(CC) -c $(CFLAGS) $^
	
clean:
	find . -maxdepth 1 \( -name "*.[ao]" -o -name "*.so" \) -exec rm {} \;
	
.PHONY: all shared static compile clean