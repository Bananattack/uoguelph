TwoBucket: TwoBucket.c Bucket.o Action.o List.o Element.o
	gcc TwoBucket.c -o TwoBucket Bucket.o Action.o List.o Element.o

Bucket.o: Bucket.c Bucket.h
	gcc Bucket.c -c

Action.o: Action.c Action.h Bucket.o List.o
	gcc Action.c -c

List.o: adt/List.c adt/List.h adt/Element.o
	gcc adt/List.c -c

Element.o: adt/Element.c adt/Element.h
	gcc adt/Element.c -c

clean:
	@ rm *.o
