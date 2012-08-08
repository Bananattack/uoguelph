compiler=gcc

compiler_flags      =	-Wall -ansi -pedantic
ld_flags     =

includes    =
library_directives     =
libs        =

adt_objects =  Element.o \
			List.o \
			Graph.o \
			Vertex.o \
			Edge.o \
			String.o
graph_menu_object = GraphMenu.o
road_object =  Road.o
analysis_object =  Analysis.o
travel_object =  Travel.o
default: all

all: GraphMenu Road Analysis Travel

GraphMenu: $(adt_objects) $(graph_menu_object)
	$(compiler) $(library_directives) $(ld_flags) -o $@ $(adt_objects) $(graph_menu_object) $(libs)
Road: $(adt_objects) $(road_object)
	$(compiler) $(library_directives) $(ld_flags) -o $@ $(adt_objects) $(road_object) $(libs)
Analysis: $(adt_objects) $(analysis_object)
	$(compiler) $(library_directives) $(ld_flags) -o $@ $(adt_objects) $(analysis_object) $(libs)
Travel: $(adt_objects) $(travel_object)
	$(compiler) $(library_directives) $(ld_flags) -o $@ $(adt_objects) $(travel_object) $(libs)

Element.o: adt/Element.c adt/Element.h
	$(compiler) $(compiler_flags) -c adt/Element.c $(includes)
List.o: adt/List.c adt/List.h adt/Element.h
	$(compiler) $(compiler_flags) -c adt/List.c $(includes)
Graph.o: adt/Graph.c adt/Graph.h adt/List.h adt/Edge.h adt/Vertex.h adt/String.h
	$(compiler) $(compiler_flags) -c adt/Graph.c $(includes)
Vertex.o: adt/Vertex.c adt/Vertex.h adt/List.h adt/Edge.h adt/__EdgeVertexPrototypes.h
	$(compiler) $(compiler_flags) -c adt/Vertex.c $(includes)
Edge.o: adt/Edge.c adt/Edge.h adt/Vertex.h adt/__EdgeVertexPrototypes.h
	$(compiler) $(compiler_flags) -c adt/Edge.c $(includes)
String.o: adt/String.c adt/String.h
	$(compiler) $(compiler_flags) -c adt/String.c $(includes)
GraphMenu.o: GraphMenu.c adt/Graph.h adt/Vertex.h adt/Edge.h adt/String.h adt/List.h
	$(compiler) $(compiler_flags) -c GraphMenu.c $(includes)
Road.o: Road.c adt/Graph.h
	$(compiler) $(compiler_flags) -c Road.c $(includes)
Analysis.o: Analysis.c adt/Graph.h
	$(compiler) $(compiler_flags) -c Analysis.c $(includes)
Travel.o: Travel.c adt/Graph.h
	$(compiler) $(compiler_flags) -c Travel.c $(includes)

clean:
	@ rm *.o
