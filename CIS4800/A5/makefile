# --------------------------
# CIS*4800 Computer Graphics
# Andrew Crowell
# acrowell@uoguelph.ca
# 0545826
# --------------------------

all: a5

# MACOS includes and libs
LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -L/opt/local/lib -lglui 
INCLUDES = -I/usr/local/include -I/opt/local/include

a5: *.h *.cpp ply.o vertex.o face.o polygon.o
	g++ a5.cpp ply.o vertex.o face.o polygon.o -o a5 -O3 -Wall $(INCLUDES) $(LIBS)

vertex.o: *.h vertex.cpp
	g++ vertex.cpp -c -o vertex.o -O3 -Wall $(INCLUDES)
    
face.o: *.h face.cpp
	g++ face.cpp -c -o face.o -O3 -Wall $(INCLUDES)

polygon.o: *.h polygon.cpp
	g++ polygon.cpp -c -o polygon.o -O3 -Wall $(INCLUDES)
	
ply.o: plyfile.c ply.h
	gcc plyfile.c -c -o ply.o -I.
	
clean:
	rm -rf a5 *.o