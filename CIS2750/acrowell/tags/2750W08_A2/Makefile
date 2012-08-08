# Targets
#  TODO: Make the build compile a static libary
#  when testing no longer required for A1.
all: libraries Dialogc.class ConfigManager.class
devbuild: all tests
clean:
	@rm *.o *.class libpm.a A1Test1 A1Test2

# Java code
Dialogc.class: Dialogc.java ConfigManager.java ConfigCompiler.java DialogcDocument.java DialogcEditorPane.java FileBrowseDialog.java
	javac Dialogc.java
ConfigManager.class: ConfigManager.java
	javac ConfigManager.java

# Assignments
ParameterManager: libraries
	
# Libraries
libraries: libpm.a libConfigManager.so
libpm.a: ParameterManager.o ParameterList.o Table.o Queue.o List.o String.o
	ar cr libpm.a ParameterManager.o ParameterList.o Table.o Queue.o List.o String.o
	ranlib libpm.a
libConfigManager.so: ConfigManager.c ConfigManager.h libpm.a
	gcc ConfigManager.c -shared -Wl,-soname,libConfigManager.so -I/usr/lib/jvm/java-6-sun/include -I/usr/lib/jvm/java-6-sun/include/linux -o libConfigManager.so -L. -lpm

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
