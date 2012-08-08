# --------------------------
# CIS*4800 Computer Graphics
# Andrew Crowell
# acrowell@uoguelph.ca
# 0545826
# --------------------------

all: a1

# MACOS includes and libs
LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -L/opt/local/lib -lglui 
INCLUDES = -I/usr/local/include -I/opt/local/include

a1: Qix/*.h Qix/qix.cpp
	g++ Qix/qix.cpp -o a1 -Wall $(INCLUDES) $(LIBS)
	
clean:
	rm a1