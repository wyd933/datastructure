#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include "list.h"

//开始的时候有必要加头结点
//在编写过程中尝试无头的方法，写到后来发现必须有个判定结尾的标志
//于是加了一个系数为零的节点作为末尾
//双向链表头尾概念一致，加入尾部，写程序想象起来困难一些，但是没有区别

void __list_add(struct list_head *new,	\
			      struct list_head *prev,	\
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
//stack
void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}
void list_del_init(struct list_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}

//初始化一个节点
Polynomial Init_Poly()
{
	Polynomial poly;
	poly = malloc(sizeof(struct node));
	poly->list_node.prev = &poly->list_node;
	poly->list_node.next = &poly->list_node;
	return poly;
}

//清除一个节点
void Free_Poly(Polynomial poly)
{
	free(poly);
}

//加法需要同指数相加，如果一个链表没有按照指数排序.
//选择一个链表，每节点循环与另一个链表同指数节点相加，
//需要循环n×n次+新链表制作时间。而且结果链表无序。
//如果每个链表先排序，按顺序相加，时间是2×t（链表排序）+ 新链表制作时间

//为了保证一致性，在队列尾插入一个为0的节点，而不是空，因为多项式不写系数为零的项
//循环链表判断队列尾很恶心，用循环队列算过度设计
//如果不是为了体验内核链表这种类似于被继承的方式，建议单链表，最好数组，数组最简单。
//因为数组可以用偏移量代表指数，存储系数，问题是内存空间开销会大

//排序好的链表，按照指数大小依次插入到新的节点
//如果不排序传入AddPolynomial函数会有BUG。
//如果不适用冒泡法，该函数会比不排序链表省时间，前提是链表排序时间复杂度要比N方小


void AddPolynomial(const Polynomial Poly1,\
				const Polynomial Poly2, Polynomial PolySum)
{
	Polynomial p1 = Poly1;
	Polynomial p2 = Poly2;
	Polynomial first, before, temp, pos;
	
	before = Init_Poly();
	first = before;
	
	while(p1->Coefficient || p2->Coefficient)
	{

		if((p1->Exponent < p2->Exponent && p1->Coefficient != 0) || p2->Coefficient == 0)
		{
			
			temp = Init_Poly();
			temp->Exponent = p1->Exponent;
			temp->Coefficient = p1->Coefficient;
			list_add(&temp->list_node, &before->list_node);
			before = temp;
			temp = NULL;
			p1 = list_entry(p1->list_node.next,struct node, list_node);

			//printf("temp%d\n", temp->Coefficient);
		}
		else if(p1->Exponent == p2->Exponent)
		{
			temp = Init_Poly();
			temp->Exponent = p1->Exponent;
			temp->Coefficient = p1->Coefficient + p2->Coefficient;
			list_add(&temp->list_node, &before->list_node);
			before = temp;
			temp = NULL;
			p1 = list_entry(p1->list_node.next,struct node, list_node);
			p2 = list_entry(p2->list_node.next,struct node, list_node);
		}
		else
		{
			temp = Init_Poly();
			temp->Exponent = p2->Exponent;
			temp->Coefficient = p2->Coefficient;
			list_add(&temp->list_node, &before->list_node);
			before = temp;
			temp = NULL;
			p2 = list_entry(p2->list_node.next,struct node, list_node);
		}	
	}

	PolySum = list_entry(first->list_node.next,struct node, list_node);
	
	for (pos = PolySum;	\
		pos->list_node.next != (&PolySum->list_node);					\
      	pos = list_next_entry(pos, list_node))
	{
		 	printf("%dx%d\n",pos->Coefficient,pos->Exponent);
 	}
	free(first);
}

//乘法肯定是实现矩阵最好了，通用性最广，但是目前写不出来
//暂时双循环做，然后排序
//后续做了排序算法在优化
//空间消耗非常大
void MulPolynomial(const Polynomial Poly1,\
				const Polynomial Poly2, Polynomial PolySum)
{
	Polynomial p1 = Poly1;
	Polynomial p2 = Poly2;
	Polynomial temp, before, first, pos, prev, next;
	before = Init_Poly();
	first = before;
	for(; p1->Coefficient; p1 = \
		list_entry(p1->list_node.next,struct node, list_node))
		for(p2 = Poly2; p2->Coefficient; p2 = \
		list_entry(p2->list_node.next,struct node, list_node))
		{
			temp = Init_Poly();
			temp->Exponent = p1->Exponent + p2->Exponent;
			temp->Coefficient = p1->Coefficient * p2->Coefficient;
			list_add(&temp->list_node, &before->list_node);
			printf("p1\t%d\t%d\n,p2\t%d\t%d\n,temp\t%d\t%d\n",\
					p1->Coefficient,p1->Exponent,\
					p2->Coefficient,p1->Exponent,\
					temp->Coefficient,temp->Exponent);
			before = temp;
			temp = NULL;
		}
		PolySum = list_entry(first->list_node.next,struct node, list_node);
		//free(first);
		Sort_Poly(&PolySum);
		prev = PolySum;
		next = list_next_entry(prev, list_node);
		//printf("%dx%d\n",next->Coefficient,next->Exponent);
		for(first = PolySum; next->Coefficient != 0;)
		{
			if(prev->Exponent == next->Exponent)
			{
				prev->Coefficient = prev->Coefficient + next->Coefficient;
				list_del_init(&next->list_node);
				free(next);
				next = list_next_entry(prev, list_node);
			}
			else
			{
				prev = next;
				next = list_next_entry(prev, list_node);
			}
		}
		for (pos = PolySum;	\
			pos->list_node.next != (&PolySum->list_node);					\
   		   	pos = list_next_entry(pos, list_node))
		{
		 	printf("%dx%d\n",pos->Coefficient,pos->Exponent);
 		}
		
}

//务必先删除链表上节点，查了好久
void Swap_Poly(Polynomial P, Polynomial N)
{
	list_del_init(&N->list_node);
	__list_add(&N->list_node,P->list_node.prev, &P->list_node);
}
//排序过程中会导致P的指针方向发生改变
//P一直冒泡到头结点的前面，所以结果是10->0->1....->9,因此有必要改变P的位置
//由原来的一级指针改为二级指针
void Sort_Poly(Polynomial *P)
{
	int ischange=1;
	Polynomial p = *P;
	Polynomial prev = p;
	Polynomial end, first;
	end = first = list_entry(p->list_node.prev,struct node, list_node);
	printf("%d\n",first->Coefficient);
	Polynomial next = list_entry(p->list_node.next,struct node, list_node);
	if(first->list_node.next == &first->list_node)
		return;

	if(next->Coefficient == 0)	
		return;
		
	while(ischange)
	{	
		ischange = 0;
		for(;next->Coefficient != end->Coefficient;)
		{
 			ischange = 1;
			if(prev->Exponent > next->Exponent)
			{ 
				printf("prev->Coefficient > next->Coefficient");
				printf("%d\t%d\t%d\n",end->Coefficient,prev->Coefficient,next->Coefficient);
				Swap_Poly(prev,next);
				next = list_entry(prev->list_node.next,struct node,list_node);
				printf("prev->Coefficient > next->Coefficient");
				printf("%d\t%d\t%d\n",end->Coefficient,prev->Coefficient,next->Coefficient);	
				//sleep(1);		
  			}
			else
			{
				prev = next;
				next = list_entry(prev->list_node.next,struct node,list_node);
				printf("prev->Coefficient < next->Coefficient");
				printf("%d\t%d\n",end->Coefficient,next->Coefficient);
			} 
		}
		printf("dddddd\n");
 		end = prev;
		prev = list_entry(first->list_node.next,struct node,list_node);
		next = list_entry(prev->list_node.next,struct node,list_node); 
	}
	printf("fuck");
	*P = list_entry(first->list_node.next,struct node,list_node);;
	printf("%d\n", p->Coefficient);
}

int main()
{
	Polynomial before, first, Poly1, Poly2, PolySum, pos, end1, end2;
	int i;
	first = before = Init_Poly();
	
	end1 = Init_Poly();
	end1->Coefficient = 0;
	end1->Exponent = 0;

	end2 = Init_Poly();
	end2->Coefficient = 0;
	end2->Exponent = 0;

	for(i = 2; i > 0; i -= 1)
	{
		Poly1 = Init_Poly();
		Poly1->Exponent = i;
		Poly1->Coefficient = i;
		list_add(&Poly1->list_node, &before->list_node);
		before = Poly1;
		Poly1 = NULL;
	}
	
	list_add(&end1->list_node, &before->list_node);

	Poly1 = list_entry(first->list_node.next,struct node,list_node);

//	list_del_init(&first->list_node);

	Sort_Poly(&Poly1);

	printf("ffff\n");
	for (pos = Poly1;	\
	     pos->list_node.next != (&Poly1->list_node);					\
	     pos = list_next_entry(pos, list_node))
	{
		printf("%dx%d\n",pos->Coefficient,pos->Coefficient);
	}

 	before = first;
	before->list_node.prev = &before->list_node;
	before->list_node.next = &before->list_node;
	
	for(i = 1; i < 4; i += 1)
	{
		Poly2 = Init_Poly();
		Poly2->Exponent = i;
		Poly2->Coefficient = i;
		list_add(&Poly2->list_node, &before->list_node);
		before = Poly2;
		Poly2 = NULL;
	}

	list_add(&end2->list_node, &before->list_node);

	Poly2 = list_entry(first->list_node.next,struct node,list_node);

	for (pos = Poly2;	\
		pos->list_node.next != (&Poly2->list_node);					\
      	pos = list_next_entry(pos, list_node))
	{
		 	printf("%dx%d\n",pos->Coefficient,pos->Coefficient);
 	}

	free(first);

	//AddPolynomial(Poly1,Poly2,PolySum);
	MulPolynomial(Poly1,Poly2,PolySum);
	
 

}