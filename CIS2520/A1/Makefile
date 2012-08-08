CC          =	gcc

CFLAGS      =	-Wall -ansi -pedantic
LDFLAGS     =

INCLUDES    =
LIBDIRS     =
LIBS        =

LISTOBJS    =	CList.o \
		ListMenu.o
STACKOBS    =	CList.o \
		Stack.o \
		StackMenu.o
QUEUEOBS    =	CList.o \
		Queue.o \
		QueueMenu.o
PARENOBS    =	CList.o \
		Stack.o \
		Paren.o

default : all

all : ListMenu StackMenu QueueMenu Paren

ListMenu : $(LISTOBJS)
	$(CC) $(LIBDIRS) $(LDFLAGS) -o $@ $(LISTOBJS) $(LIBS)
StackMenu : $(STACKOBS)
	$(CC) $(LIBDIRS) $(LDFLAGS) -o $@ $(STACKOBS) $(LIBS)
QueueMenu : $(QUEUEOBS)
	$(CC) $(LIBDIRS) $(LDFLAGS) -o $@ $(QUEUEOBS) $(LIBS)
Paren: $(PARENOBS)
	$(CC) $(LIBDIRS) $(LDFLAGS) -o $@ $(PARENOBS) $(LIBS)

CList.o: adt/CList.c adt/CList.h
	$(CC) $(CFLAGS) -c adt/CList.c $(INCLUDES)
ListMenu.o: ListMenu.c adt/CList.h
	$(CC) $(CFLAGS) -c ListMenu.c $(INCLUDES)
Stack.o: adt/Stack.c adt/Stack.h adt/CList.h
	$(CC) $(CFLAGS) -c adt/Stack.c $(INCLUDES)
StackMenu.o: StackMenu.c adt/Stack.h
	$(CC) $(CFLAGS) -c StackMenu.c $(INCLUDES)
Queue.o: adt/Queue.c adt/Queue.h adt/CList.h
	$(CC) $(CFLAGS) -c adt/Queue.c $(INCLUDES)
QueueMenu.o: QueueMenu.c adt/Queue.h
	$(CC) $(CFLAGS) -c QueueMenu.c $(INCLUDES)
Paren.o: Paren.c adt/Stack.h adt/CList.h
	$(CC) $(CFLAGS) -c Paren.c $(INCLUDES)

clean:
	@ rm *.o
