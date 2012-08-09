# Makefile for HPCS-09 labs @ HPCVL, Kingston

PILOTHOME = /opt/pilot
CC = mpicc
CXX = mpicxx
AR = ar
RANLIB = ranlib
CPPFLAGS = -I$(PILOTHOME)/include
LDFLAGS =  -ljpeg -lirc -L$(PILOTHOME)/lib -lpilot
A1_REQUIREMENTS = \
	thumbpeg.o \
	thumbnailer.o \
	compressor.o

# "make labN" will compile labN.c using implicit make rules
thumbpeg: $(A1_REQUIREMENTS) thumbpeg.cpp thumbpeg.h thumbnailer.cpp compressor.cpp
	$(CXX) $(A1_REQUIREMENTS) $(LDFLAGS) $(CPPFLAGS) -o thumbpeg

thumbpeg.o: thumbpeg.cpp thumbpeg.h
	$(CXX) thumbpeg.cpp $(CPPFLAGS) -c -o thumbpeg.o

thumbnailer.o: thumbnailer.cpp thumbpeg.h
	$(CXX) thumbnailer.cpp $(CPPFLAGS) -c -o thumbnailer.o

compressor.o: compressor.cpp thumbpeg.h
	$(CXX) compressor.cpp $(CPPFLAGS) -c -o compressor.o

clean:
	$(RM) *.o thumbpeg
