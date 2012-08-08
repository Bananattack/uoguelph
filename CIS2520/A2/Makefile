compiler=gcc

compiler_flags      =	-Wall -ansi -pedantic
ld_flags     =

includes    =
library_directives     =
libs        =

list_objects    =	Element.o \
			List.o \
			OrderedList.o \
			OrderedListMenu.o
word_count_objects =	Element.o \
			List.o \
			Table.o \
			WordCount.o

default: all

all: OrderedListMenu WordCount

OrderedListMenu: $(list_objects)
	$(compiler) $(library_directives) $(ld_flags) -o $@ $(list_objects) $(libs)
WordCount: $(word_count_objects)
	$(compiler) $(library_directives) $(ld_flags) -o $@ $(word_count_objects) $(libs)

Element.o: adt/Element.c adt/Element.h
	$(compiler) $(compiler_flags) -c adt/Element.c $(includes)
List.o: adt/List.c adt/List.h adt/Element.h
	$(compiler) $(compiler_flags) -c adt/List.c $(includes)
OrderedList.o: adt/OrderedList.c adt/OrderedList.h adt/List.h
	$(compiler) $(compiler_flags) -c adt/OrderedList.c $(includes)
OrderedListMenu.o: OrderedListMenu.c adt/OrderedList.h
	$(compiler) $(compiler_flags) -c OrderedListMenu.c $(includes)
Table.o: adt/Table.c adt/Table.h adt/List.h
	$(compiler) $(compiler_flags) -c adt/Table.c $(includes)
WordCount.o: WordCount.c adt/Table.h
	$(compiler) $(compiler_flags) -c WordCount.c $(includes)

clean:
	@ rm *.o
