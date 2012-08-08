# Targets
#  TODO: Make the build compile a static libary
#  when testing no longer required for A1.
all: libraries yadc Dialogc.class ConfigManager.class
devbuild: all tests
clean:
	@rm *.o *.class libpm.a A1Test1 A1Test2 yadc

# Java code
Dialogc.class: Dialogc.java ConfigManager.java ConfigCompiler.java DialogcDocument.java DialogcEditorPane.java FileBrowseDialog.java
	javac Dialogc.java
ConfigManager.class: ConfigManager.java
	javac ConfigManager.java

# Assignments
ParameterManager: libraries
	
# Libraries
libraries: libpm.a libJNIpm.so
libpm.a: ParameterManager.o ParameterList.o Table.o Queue.o List.o String.o
	ar cr libpm.a ParameterManager.o ParameterList.o Table.o Queue.o List.o String.o
	ranlib libpm.a
libJNIpm.so: ConfigManager.c ConfigManager.h libpm.a
	gcc ConfigManager.c -shared -Wl,-soname,libJNIpm.so -I/usr/lib/jvm/java-6-sun/include -I/usr/lib/jvm/java-6-sun/include/linux -o libJNIpm.so -L. -lpm

# Dialogc external compiler
yadc: yacc.o lex.o Table.o Queue.o List.o String.o
	gcc lex.o yacc.o Table.o Queue.o List.o String.o -o yadc -ll -ly
lex.o: yadc.l
	lex yadc.l
	gcc lex.yy.c -c -o lex.o
yacc.o: yadc.y
	yacc yadc.y -d -y
	gcc y.tab.c -c -o yacc.o

# Test programs
tests: A1Test1 A1Test2
A1Test1: A1Test1.c libraries
	gcc A1Test1.c -o A1Test1 -L. -lpm
A1Test2: A1Test2.c libraries
	gcc A1Test2.c -o A1Test2 -L. -lpm

# libpm object files
ParameterManager.o: ParameterManager.c ParameterManager.h ParameterList.h adt/Boolean.h adt/Queue.h adt/Table.h adt/String.h
	gcc ParameterManager.c -c
ParameterList.o: ParameterList.c ParameterList.h
	gcc ParameterList.c -c
Table.o: adt/Table.c adt/Table.h adt/List.h
	gcc adt/Table.c -c
Queue.o: adt/Queue.c adt/Queue.h adt/List.h
	gcc adt/Queue.c -c
List.o: adt/List.c adt/List.h
	gcc adt/List.c -c
String.o: adt/String.c adt/String.h
	gcc adt/String.c -c
