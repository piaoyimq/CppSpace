#include<stdio.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0

/* 二叉排序树的节点结构定义 */
typedef struct BiTNode
{
	int data;
	struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

int SearchBST(BiTree T, int key, BiTree f, BiTree *p)
{
	/* 递归查找二叉排序树T中是否存在key */
	/* 指针f指向T的双亲，其初始调用值为NULL */
	/* 若查找成功，则指针p指向该数据元素节点，并返回TRUE */
	/* 否则指针p指向查找路径上访问的最后一个节点并返回FALSE */
	if (!T)
	{
		*p = f;     //这是f唯一被用到的位置。
		return FALSE;
	}
	else
	{
		if (key == T->data)
		{
			*p = T;
			return TRUE;
		}
		else
			if (key > T->data)
				return SearchBST(T->rchild, key, T, p); /* 在右子树继续查找 */
			else
				return SearchBST(T->lchild, key, T, p); /* 在左子树继续查找 */
	}
}

int SearchBST2(BiTree T, int key, BiTree f, BiTree *p)
{
	/*非递归*/
	BiTree s;
	if (!T)
	{
		*p = f;
		return FALSE;
	}
	else
	{
		while (T)
		{
			if (key == T->data)
			{
				*p = T;
				return TRUE;
			}
			if (key > T->data)
			{
				s = T;
				T = T->rchild;
			}
			else
			{
				s = T;
				T = T->lchild;
			}
		}
		*p = s;
		return FALSE;
	}
}

int InsertBST1(BiTree *T, int key)
{
	/* 当二叉排序树T中不存在关键字等于key的数据元素时 */
	/* 插入key并返回TRUE，否则返回FALSE */
	/* 调用查找函数SearchBST，非递归 */
	BiTree p, s;
	if (!SearchBST2(*T, key, NULL, &p))
	{
		s = (BiTree) malloc(sizeof(BiTNode));
		s->data = key;
		s->lchild = s->rchild = NULL;
		if (!p)
			*T = s; /* 插入s为根节点，此前树为空树 */
		else
			if (key > p->data)
				p->rchild = s; /* 插入s为右孩子 */
			else
				p->lchild = s; /* 插入s为左孩子 */
		return TRUE;
	}
	return FALSE;
}

int InsertBST2(BiTree *T, int key)
{
	/* 当二叉排序树T中不存在关键字等于key的数据元素时 */
	/* 插入key并返回TRUE，否则返回FALSE */
	/* 未调用查找函数，递归插入 */
	if (!(*T)) /* 树为空， */
	{
		(*T) = (BiTree) malloc(sizeof(BiTNode)); /* 这个位置要留心，要重新分配空间，*T为空，说明未曾分配空间 */
		(*T)->data = key;
		(*T)->lchild = (*T)->rchild = NULL;
		return TRUE;
	}
	if (key == (*T)->data)
		return FALSE;
	if (key > (*T)->data)
		return InsertBST2(&((*T)->rchild), key); /* 插入右孩子 */
	else
		return InsertBST2(&((*T)->lchild), key); /* 插入左孩子 */
}

void order(BiTree t)     //中序输出
{
	if (t == NULL)
		return;
	order(t->lchild);
	printf("%d ", t->data);
	order(t->rchild);
}

int Delete(BiTree *p)
{
	/* 从二叉排序树中删除节点p， 并重接它的左或右子树 */
	BiTree q, s;
	if (!(*p)->lchild && !(*p)->rchild) /* p为叶子节点 */
		*p = NULL;
	else
		if (!(*p)->lchild) /* 左子树为空，重接右子树 */
		{
			q = *p;
			*p = (*p)->rchild;
			free(q);
		}
		else
			if (!(*p)->rchild) /* 右子树为空，重接左子树 */
			{
				q = *p;
				*p = (*p)->lchild; /* 不太理解 */
				free(q);
			}
			else /* 左右子树均不为空 */
			{
				q = *p;
				s = (*p)->lchild;
				while (s->rchild) /* 转左，然后向右走到尽头*/
				{
					q = s;
					s = s->rchild;
				}
				(*p)->data = s->data;
				if (q != *p) /* 判断是否执行上述while循环 */
					q->rchild = s->lchild; /* 执行上述while循环，重接右子树 */
				else
					q->lchild = s->lchild; /* 未执行上述while循环，重接左子树 */
				free(s);
			}
	return TRUE;
}

int DeleteBST(BiTree *T, int key)
{
	/* 若二叉排序树T中存在关键字等于key的数据元素时，则删除该数据元素节点 */
	/* 并返回TRUE；否则返回FALSE */
	if (!(*T))
		return FALSE; /* 不存在关键字等于key的数据元素 */
	else
	{
		if (key == (*T)->data)
			Delete(T);
		else
			if (key < (*T)->data)
				return DeleteBST(&(*T)->lchild, key);
			else
				return DeleteBST(&(*T)->rchild, key);
	}
}


int main()
{
	int i;
	int a[10] = { 62, 88, 58, 47, 35, 73, 51, 99, 37, 93 };
	BiTree T = NULL;
	for (i = 0; i < 10; i++)
		InsertBST1(&T, a[i]);
	printf("中序遍历二叉排序树：\n");
	order(T);
	printf("\n");
	printf("删除58后，中序遍历二叉排序树：\n");
	DeleteBST(&T, 58);
	order(T);
	printf("\n");

	return 0;
}
