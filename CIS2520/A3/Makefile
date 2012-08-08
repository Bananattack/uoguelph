all: 8queens Freq

8queens: 8queens.c
	gcc 8queens.c -o 8queens

Freq: Freq.o BinaryTree.o AVLTree.o BinaryTreeNode.o
	gcc Freq.c -o Freq BinaryTree.o BinaryTreeNode.o AVLTree.o

Freq.o: Freq.c adt/AVLTree.h adt/BinaryTree.h
	gcc 8queens.c -o 8queens

AVLTree.o: adt/AVLTree.c adt/AVLTree.h adt/BinaryTree.h
	gcc adt/AVLTree.c -c

BinaryTree.o: adt/BinaryTree.c adt/BinaryTree.h adt/BinaryTreeNode.h
	gcc adt/BinaryTree.c -c

BinaryTreeNode.o: adt/BinaryTreeNode.c adt/BinaryTreeNode.h
	gcc adt/BinaryTreeNode.c -c

clean:
	@ rm *.o
