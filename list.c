#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include "list.h"

void __list_add(struct list_head *new,	\
			      struct list_head *prev,	\
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

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

Polynomial Init_Poly()
{
	Polynomial poly;
	poly = malloc(sizeof(struct node));
	poly->list_node.prev = &poly->list_node;
	poly->list_node.next = &poly->list_node;
	return poly;
}

void Del_Poly_List(Polynomial poly)
{
	free(poly);
}

Polynomial AddPolynomial(const Polynomial Poly1,\
				const Polynomial Poly2, Polynomial PolySum)
{
	Polynomial p1 = list_entry(Poly1->list_node.next,struct node, list_node);
	Polynomial p2 = list_entry(Poly2->list_node.next,struct node, list_node);
	Polynomial first, before, temp, pos;
	first = PolySum;
	while(p1->Coefficient || p2->Coefficient)
	{
		if((p1->Exponent < p2->Exponent && p1->Coefficient != 0) || p2->Coefficient == 0)
		{
			
			temp = Init_Poly();
			temp->Exponent = p1->Exponent;
			temp->Coefficient = p1->Coefficient;
			list_add(&temp->list_node, &PolySum->list_node);
			PolySum= temp;
			temp = NULL;
			p1 = list_entry(p1->list_node.next,struct node, list_node);
		}
		else if(p1->Exponent == p2->Exponent)
		{
			temp = Init_Poly();
			temp->Exponent = p1->Exponent;
			temp->Coefficient = p1->Coefficient + p2->Coefficient;
			list_add(&temp->list_node, &PolySum->list_node);
			PolySum = temp;
			temp = NULL;
			p1 = list_entry(p1->list_node.next,struct node, list_node);
			p2 = list_entry(p2->list_node.next,struct node, list_node);
		}
		else
		{
			temp = Init_Poly();
			temp->Exponent = p2->Exponent;
			temp->Coefficient = p2->Coefficient;
			list_add(&temp->list_node, &PolySum->list_node);
			PolySum = temp;
			temp = NULL;
			p2 = list_entry(p2->list_node.next,struct node, list_node);
		}	
	}
	return first;
}

void MulPolynomial(const Polynomial Poly1,\
				const Polynomial Poly2, Polynomial PolyMul)
{
	Polynomial head1 = Poly1;
	Polynomial head2 = Poly2;
	Polynomial headMul = PolyMul;
	Polynomial p1,p2;
	Polynomial temp, before, first, pos, prev, next;
	for(p1 = list_entry(head1->list_node.next,struct node, list_node);\
		 p1->Coefficient; 	\
		 p1 = list_entry(p1->list_node.next,struct node, list_node))
		for(p2 = list_entry(head2->list_node.next,struct node, list_node);\
			 p2->Coefficient != 0; \
			 p2 = list_entry(p2->list_node.next,struct node, list_node))
		{
			temp = Init_Poly();
			temp->Exponent = p1->Exponent + p2->Exponent;
			temp->Coefficient = p1->Coefficient * p2->Coefficient;
			list_add(&temp->list_node, &PolyMul->list_node);
			PolyMul = temp;
			temp = NULL;
		}
		PolyMul = Sort_Poly(headMul);
		
		prev = PolyMul;
		next = list_next_entry(prev, list_node);
		for(; next->Coefficient != 0;)
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
 		for (pos = list_entry(PolyMul->list_node.next,struct node,list_node);	\
	     	pos->Coefficient != 0;					\
	     	pos = list_next_entry(pos, list_node))
		{
			printf("%dx%d\n",pos->Coefficient,pos->Exponent);
		} 
		
}

void Swap_Poly(Polynomial P, Polynomial N)
{
	list_del_init(&N->list_node);
	__list_add(&N->list_node,P->list_node.prev, &P->list_node);
}

Polynomial Sort_Poly(Polynomial p)
{
	int ischange=1;
	Polynomial end, first;
	end = first = p;
	Polynomial prev = list_entry(p->list_node.next,struct node, list_node);
	Polynomial next = list_entry(prev->list_node.next,struct node, list_node);
	
	if(first->list_node.next == &first->list_node)
		return NULL;

	if(prev->list_node.next == &first->list_node)	
		return NULL;
		
	while(ischange)
	{	
		ischange = 0;
		for(;next->Coefficient != end->Coefficient;)
		{
 			ischange = 1;
			if(prev->Exponent > next->Exponent)
			{ 
				Swap_Poly(prev,next);
				next = list_entry(prev->list_node.next,struct node,list_node);	
  			}
			else
			{
				prev = next;
				next = list_entry(prev->list_node.next,struct node,list_node);
			} 
		}
 		end = prev;
		prev = list_entry(first->list_node.next,struct node,list_node);
		next = list_entry(prev->list_node.next,struct node,list_node); 
	}
	return first;
}

int main()
{
	Polynomial Poly1, Poly2, PolySum, pos, head1, head2, temp, headSum, PolyMul;
	int i;

	Poly1 = Init_Poly();
	Poly1->Exponent = 0;
	Poly1->Coefficient = 0;
	head1 = Poly1;
	
	Poly2 = Init_Poly();
	Poly2->Coefficient = 0;
	Poly2->Exponent = 0;
	head2 = Poly2;

	PolySum = Init_Poly();
	PolySum->Coefficient = 0;
	PolySum->Exponent = 0;
	headSum = PolySum;

	PolyMul = Init_Poly();
	PolyMul->Coefficient = 0;
	PolyMul->Exponent = 0;
	PolyMul = PolySum;

	for(i = 3; i > 0; i -= 1)
	{
		temp = Init_Poly();
		temp->Exponent = i;
		temp->Coefficient = i;
		list_add(&temp->list_node, &Poly1->list_node);
		Poly1 = temp;
		temp = NULL;
	}
	
	Poly1 = Sort_Poly(head1);

	for (pos = list_entry(head1->list_node.next,struct node,list_node);	\
	     pos->Coefficient != 0;					\
	     pos = list_next_entry(pos, list_node))
	{
		printf("%dx%d\n",pos->Coefficient,pos->Coefficient);
	}


	for(i = 1; i < 3; i += 1)
	{
		temp = Init_Poly();
		temp->Exponent = i;
		temp->Coefficient = i;
		list_add(&temp->list_node, &Poly2->list_node);
		Poly2 = temp;
		temp = NULL;
		printf("ddd%d\n", Poly2->Coefficient);
	}
	
	Poly2 = Sort_Poly(head2);

	for (pos = list_entry(head2->list_node.next,struct node,list_node);	\
	     pos->Coefficient != 0;					\
	     pos = list_next_entry(pos, list_node))
	{
		printf("%dx%d\n",pos->Coefficient,pos->Coefficient);
	}

	//PolySum = AddPolynomial(Poly1,Poly2,PolySum);
	MulPolynomial(Poly1,Poly2,PolySum); 
}