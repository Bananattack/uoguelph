# --------------------------
# CIS*4800 Computer Graphics
# Andrew Crowell
# acrowell@uoguelph.ca
# 0545826
# --------------------------

all: a4

# MACOS includes and libs
LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -L/opt/local/lib -lglui 
INCLUDES = -I/usr/local/include -I/opt/local/include

a4: *.h a4.cpp heightmap.cpp
	g++ a4.cpp heightmap.cpp -g -o a4 -Wall $(INCLUDES) $(LIBS)
	
clean:
	rm a4