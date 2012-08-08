# --------------------------
# CIS*4800 Computer Graphics
# Andrew Crowell
# acrowell@uoguelph.ca
# 0545826
# --------------------------


APPLICATION_NAME = a2
all: $(APPLICATION_NAME)

# MACOS includes and libs
LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -L/opt/local/lib -lglui 
INCLUDES = -I/usr/local/include -I/opt/local/include

$(APPLICATION_NAME): Qix/*.h Qix/qix.cpp
	g++ Qix/qix.cpp -o $(APPLICATION_NAME) -Wall $(INCLUDES) $(LIBS)
	
clean:
	rm $(APPLICATION_NAME)