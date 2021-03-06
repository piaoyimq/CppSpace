#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_

#include "dsexceptions.h"
#include <iostream>    // For NULL

using namespace std;

// Binary node and forward declaration because g++ does
// not understand nested classes.
template<class Comparable>
class BinarySearchTree;

template<class Comparable>
class BinaryNode
{
	Comparable element;
	BinaryNode *left;
	BinaryNode *right;

	BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt) :
			element(theElement), left(lt), right(rt)
	{
	}
	friend class BinarySearchTree<Comparable> ;
};

// BinarySearchTree class
//
// CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// Comparable find( x )   --> Return item that matches x
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order

template<class Comparable>
class BinarySearchTree
{
public:
	explicit BinarySearchTree(const Comparable & notFound);
	BinarySearchTree(const BinarySearchTree & rhs);
	~BinarySearchTree();
	const Comparable & findMin() const;
	const Comparable & findMax() const;
	const Comparable & find(const Comparable & x) const;
	bool isEmpty() const;
	void printTree() const;

	void makeEmpty();
	void insert(const Comparable & x);
	void remove(const Comparable & x);

	const BinarySearchTree & operator=(const BinarySearchTree & rhs);

	int hight_tree(const BinarySearchTree<Comparable>* t); // get the height of the tree.
	bool is_equal(const BinarySearchTree<Comparable>* t1, const BinarySearchTree<Comparable>* t2); //compare 2 tree if equral.
	int count_tree(bintree t);
	//layer traversal  ??

	// how to judge a tree is a BST ?

private:
	BinaryNode<Comparable> *root;
	const Comparable ITEM_NOT_FOUND;

	const Comparable & elementAt(BinaryNode<Comparable> *t) const;

	void insert(const Comparable & x, BinaryNode<Comparable> * & t) const;
	void remove(const Comparable & x, BinaryNode<Comparable> * & t) const; //?
	BinaryNode<Comparable> * findMin(BinaryNode<Comparable> *t) const;
	BinaryNode<Comparable> * findMax(BinaryNode<Comparable> *t) const;
	BinaryNode<Comparable> * find(const Comparable & x, BinaryNode<Comparable> *t) const;
	void makeEmpty(BinaryNode<Comparable> * & t) const;
	void printTree(BinaryNode<Comparable> *t) const;
	BinaryNode<Comparable> * clone(BinaryNode<Comparable> *t) const;
};

#include <iostream>

using namespace std;

/**
 * Implements an unbalanced binary search tree.
 * Note that all "matching" is based on the < method.
 */

/**
 * Construct the tree.
 */
template<class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound) :
		ITEM_NOT_FOUND(notFound), root(NULL)
{
}

template<class Comparable>
int BinarySearchTree<Comparable>::hight_tree(const BinarySearchTree<Comparable>* t)
{
	int h, left, right;
	if (!t)
	{
		return 0;
	}
	left = hight_tree(t->left);
	right = hight_tree(t->right);
	h = (left > right ? left : right) + 1;
	return h;
}

template<class Comparable>
bool BinarySearchTree<Comparable>::is_equal(const BinarySearchTree<Comparable>* t1, const BinarySearchTree<Comparable>* t2)
{
	if (!t1 && !t2)
	{      //都为空就相等
		return true;
	}
	if (t1 && t2 && t1->data == t2->data)
	{      //有一个为空或数据不同就不判断了
		if (is_equal(t1->lchild, t2->lchild))
			if (is_equal(t1->rchild, t2->rchild))
			{
				return true;
			}
	}
	return false;
}

template<class Comparable>
int BinarySearchTree<Comparable>::count_tree(bintree t)
{		//cacluate the number of the node.
	if (t)
	{
		return (count_tree(t->lchild) + count_tree(t->rchild) + 1);
	}
	return 0;
}

/**
 * Copy constructor.
 */
template<class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const BinarySearchTree<Comparable> & rhs) :
		root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND)
{
	*this = rhs;
}

/**
 * Destructor for the tree.
 */
template<class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree()
{
	makeEmpty();
}

/**
 * Insert x into the tree; duplicates are ignored.
 */
template<class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x)
{
	insert(x, root);
}

/**
 * Remove x from the tree. Nothing is done if x is not found.
 */
template<class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x)
{
	remove(x, root);
}

/**
 * Find the smallest item in the tree.
 * Return smallest item or ITEM_NOT_FOUND if empty.
 */
template<class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMin() const
{
	return elementAt(findMin(root));
}

/**
 * Find the largest item in the tree.
 * Return the largest item of ITEM_NOT_FOUND if empty.
 */
template<class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMax() const
{
	return elementAt(findMax(root));
}

/**
 * Find item x in the tree.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template<class Comparable>
const Comparable & BinarySearchTree<Comparable>::find(const Comparable & x) const
{
	return elementAt(find(x, root));
}

/**
 * Make the tree logically empty.
 */
template<class Comparable>
void BinarySearchTree<Comparable>::makeEmpty()
{
	makeEmpty(root);
}

/**
 * Test if the tree is logically empty.
 * Return true if empty, false otherwise.
 */
template<class Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const
{
	return root == NULL;
}

/**
 * Print the tree contents in sorted order.
 */
template<class Comparable>
void BinarySearchTree<Comparable>::printTree() const
{
	if (isEmpty())
		cout << "Empty tree" << endl;
	else
		printTree(root);
}

/**
 * Deep copy.
 */
template<class Comparable>
const BinarySearchTree<Comparable> &
BinarySearchTree<Comparable>::operator=(const BinarySearchTree<Comparable> & rhs)
{
	if (this != &rhs)
	{
		makeEmpty();
		root = clone(rhs.root);
	}
	return *this;
}

/**
 * Internal method to get element field in node t.
 * Return the element field or ITEM_NOT_FOUND if t is NULL.
 */
template<class Comparable>
const Comparable & BinarySearchTree<Comparable>::elementAt(BinaryNode<Comparable> *t) const
{
	if (t == NULL)
		return ITEM_NOT_FOUND;
	else
		return t->element;
}

/**
 * Internal method to insert into a subtree.
 * x is the item to insert.
 * t is the node that roots the tree.
 * Set the new root.
 */
template<class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x, BinaryNode<Comparable> * & t) const
{
	if (t == NULL)
		t = new BinaryNode<Comparable>(x, NULL, NULL);
	else
		if (x < t->element)
			insert(x, t->left);
		else
			if (t->element < x)
				insert(x, t->right);
			else
				;  // Duplicate; do nothing
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the tree.
 * Set the new root.
 */
template<class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x, BinaryNode<Comparable> * & t) const
{
	if (t == NULL)
		return;   // Item not found; do nothing
	if (x < t->element)
		remove(x, t->left);
	else
		if (t->element < x)
			remove(x, t->right);
		else
			if (t->left != NULL && t->right != NULL) // Two children
			{
				t->element = findMin(t->right)->element;
				remove(t->element, t->right);
			}
			else
			{
				BinaryNode<Comparable> *oldNode = t;
				t = (t->left != NULL) ? t->left : t->right;
				delete oldNode;
			}
}

/**
 * Internal method to find the smallest item in a subtree t.
 * Return node containing the smallest item.
 */
template<class Comparable>
BinaryNode<Comparable> *
BinarySearchTree<Comparable>::findMin(BinaryNode<Comparable> *t) const
{
	if (t == NULL)
		return NULL;
	if (t->left == NULL)
		return t;
	return findMin(t->left);
}

/**
 * Internal method to find the largest item in a subtree t.
 * Return node containing the largest item.
 */
template<class Comparable>
BinaryNode<Comparable> *
BinarySearchTree<Comparable>::findMax(BinaryNode<Comparable> *t) const
{
	if (t != NULL)
		while (t->right != NULL)
			t = t->right;
	return t;
}

/**
 * Internal method to find an item in a subtree.
 * x is item to search for.
 * t is the node that roots the tree.
 * Return node containing the matched item.
 */
template<class Comparable>
BinaryNode<Comparable> *
BinarySearchTree<Comparable>::find(const Comparable & x, BinaryNode<Comparable> *t) const
{
	if (t == NULL)
		return NULL;
	else
		if (x < t->element)
			return find(x, t->left);
		else
			if (t->element < x)
				return find(x, t->right);
			else
				return t;    // Match
}
/****** NONRECURSIVE VERSION*************************
 template <class Comparable>
 BinaryNode<Comparable> *
 BinarySearchTree<Comparable>::
 find( const Comparable & x, BinaryNode<Comparable> *t ) const
 {
 while( t != NULL )
 if( x < t->element )
 t = t->left;
 else if( t->element < x )
 t = t->right;
 else
 return t;    // Match

 return NULL;   // No match
 }
 *****************************************************/

/**
 * Internal method to make subtree empty.
 */
template<class Comparable>
void BinarySearchTree<Comparable>::makeEmpty(BinaryNode<Comparable> * & t) const
{
	if (t != NULL)
	{
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
	t = NULL;
}

/**
 * Internal method to print a subtree rooted at t in sorted order.
 */
template<class Comparable>
void BinarySearchTree<Comparable>::printTree(BinaryNode<Comparable> *t) const
{
#if 1//inorder traversal
	if (t != NULL)
	{
		printTree(t->left);
		cout << t->element << endl;
		printTree(t->right);
	}
#elif 1 //preorder traversal
	if (t != NULL)
	{
		cout << t->element << endl;
		printTree(t->left);
		printTree(t->right);
	}
#elif 1 //postorder traversal
	if (t != NULL)
	{
		printTree(t->left);
		printTree(t->right);
		cout << t->element << endl;
	}
#endif
}

/**
 * Internal method to clone subtree.
 */
template<class Comparable>
BinaryNode<Comparable> *
BinarySearchTree<Comparable>::clone(BinaryNode<Comparable> * t) const
{
	if (t == NULL)
		return NULL;
	else
		return new BinaryNode<Comparable>(t->element, clone(t->left), clone(t->right));
}
#endif
