all: Lab5

Lab5: Lab5.o BinaryTree.o BinaryTreeNode.o List.o Element.o
	gcc Lab5.c -o Lab5 BinaryTree.o BinaryTreeNode.o List.o Element.o

Lab5.o: Lab5.c adt/BinaryTree.h
	gcc Lab5.c -c

BinaryTree.o: adt/BinaryTree.c adt/BinaryTree.h adt/BinaryTreeNode.h adt/List.h
	gcc adt/BinaryTree.c -c

BinaryTreeNode.o: adt/BinaryTreeNode.c adt/BinaryTreeNode.h adt/List.h
	gcc adt/BinaryTreeNode.c -c

List.o: adt/List.c adt/List.h adt/Element.h
	gcc adt/List.c -c

Element.o: adt/Element.c adt/Element.h
	gcc adt/Element.c -c

clean:
	@ rm *.o
