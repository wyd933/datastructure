//error
//expected ‘;’ before ‘}’ token
// #define LIST_HEAD_INIT(name) { &(name), &(name) }


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

struct list_head 
{
    struct list_head *prev, *next;
};

struct node
{
    int Coefficient;
    int Exponent;
    struct list_head list_node;
};
typedef struct node *Polynomial;

//#define offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	((type *)(__mptr - offsetof(type, member))); })

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, typeof(*(pos)), member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)


#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_next_entry(pos, member))

void __list_add(struct list_head *new,	\
			      struct list_head *prev,	\
			      struct list_head *next);

void list_add(struct list_head *new, struct list_head *head);

Polynomial Init_Poly();

void Sort_Poly(Polynomial *P);

inline void AddPolynomial(const Polynomial Poly1,\
				const Polynomial Poly2, Polynomial PolySum);

