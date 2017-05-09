#include <iostream>
#include "BinarySearchTree.h"

using namespace std;

void test_tree_1()
{
	const int ITEM_NOT_FOUND = -9999;
	BinarySearchTree<int> t(ITEM_NOT_FOUND);
	int NUMS = 4000;
	const int GAP = 37;
	int i;

	cout << "Checking... (no more output means success)" << endl;

	for (i = GAP; i != 0; i = (i + GAP) % NUMS)
		t.insert(i);

	for (i = 1; i < NUMS; i += 2)
		t.remove(i);

	if (NUMS < 40)
		t.printTree();
	if (t.findMin() != 2 || t.findMax() != NUMS - 2)
		cout << "FindMin or FindMax error!" << endl;

	for (i = 2; i < NUMS; i += 2)
		if (t.find(i) != i)
			cout << "Find error1!" << endl;

	for (i = 1; i < NUMS; i += 2)
	{
		if (t.find(i) != ITEM_NOT_FOUND)
			cout << "Find error2!" << endl;
	}

	BinarySearchTree<int> t2(ITEM_NOT_FOUND);
	t2 = t;

	for (i = 2; i < NUMS; i += 2)
		if (t2.find(i) != i)
			cout << "Find error1!" << endl;

	for (i = 1; i < NUMS; i += 2)
	{
		if (t2.find(i) != ITEM_NOT_FOUND)
			cout << "Find error2!" << endl;
	}
}

void test_tree()
{
	/*
	 * 							50
	 *                 30                        80
	 *          20          40	                            90
	 *                     35                          85
	 *                  32                                      88
	 *
	 * */

	const int ITEM_NOT_FOUND = -9999;
	BinarySearchTree<int> t(ITEM_NOT_FOUND);
	t.insert(50);
	t.insert(40);
	t.insert(30);
	t.insert(20);
	t.insert(35);
	t.insert(32);
	t.insert(80);
	t.insert(90);
	t.insert(85);
	t.insert(88);

	std::cout << std::string(50, '*') << std::endl;
	t.printTree();

	t.remove(88);
	std::cout << std::string(50, '*') << std::endl;
	t.printTree();
	t.insert(88);

	t.remove(90);
	std::cout << std::string(50, '*') << std::endl;
	t.printTree();
	t.insert(90);

	t.remove(50);
	std::cout << std::string(50, '*') << std::endl;
	t.printTree();
	t.insert(50);


	std::cout << std::string(50, '*') << std::endl;
	t.printTree();

	std::cout << "max=" << t.findMax() << std::endl;

	std::cout << "min=" << t.findMin() << std::endl;

	t.makeEmpty();
	std::cout << std::string(50, '*') << std::endl;
	t.printTree();
}

// Test program
int main()
{
//	test_tree_1();
	test_tree();

	return 0;
}
