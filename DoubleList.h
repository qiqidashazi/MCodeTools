//
///内核双向链表调用
//使用说明：将LIST_ENTRY对象添加至用户自定义的结构体内当做成员进行使用

#ifndef _DOUBLELIST_H_
#define _DOUBLELIST_H_


//根据传入的 --成员地址--结构体名称--成员变量名 找出内存中该成员整个结构所在内存地址。
//返回指针类型为该结构体类型

#define CONTAINER_SEARCH(ptr,type,member)	(\
	(type*)((char*)ptr - ((ULONG)(&((type*)0)->member)))\
)


//双向链表
typedef struct LIST_ENTRY{
	struct LIST_ENTRY *	Flink;  //forward link 向前的节点 == 下一个节点
	struct LIST_ENTRY *	Blink;  //behind  link 向后的节点 == 前一个节点
}LIST_ENTRY, *PLIST_ENTRY;

//返回类型 void 
#define InitializeListHead(head)  ((head)->Flink = (head)->Blink = (head)) 

//返回类型 bool
#define IsListEmpty(head) ((head)->Flink == (head))

//返回类型 void
#define	RemoveEntryList(Entry)			\
	{\
		PLIST_ENTRY	Flink;\
		PLIST_ENTRY	Blink;\
		Flink = (Entry)->Flink;\
		Blink = (Entry)->Blink;\
		Blink->Flink = Flink;\
		Flink->Blink = Blink;\
	}

//返回类型 PLIST_ENTRY  下面remove只做连接前后指针的操作，需要定义Flink作为可返回的指针
#define	RemoveHeadList(head)	\
		(head)->Flink; \ 
		{RemoveEntryList((head)->Flink)}

//返回类型 PLIST_ENTRY  同上
#define	RemoveTailList(head)		\
		(head)->Blink;\
		{RemoveEntryList((head)->Blink)}

//返回类型 void	Blink记录前一个节点的位置
#define	InsertTailList(ListHead,Entry)	\
	{\
		PLIST_ENTRY	pHead;\
		PLIST_ENTRY	Blink;\
		pHead = (ListHead);\
		Blink = pHead->Blink;\
		(Entry)->Flink = pHead;\
		(Entry)->Blink = Blink;\
		Blink->Flink = (Entry);\
		pHead->Blink = (Entry);\
	}

//返回类型 void FLink记录下一个节点位置
#define	InsertHeadList(ListHead,Entry)	\
	{\
		PLIST_ENTRY	pHead;\
		PLIST_ENTRY	Flink;\
		pHead = (ListHead);\
		Flink = pHead->Flink;\
		(Entry)->Flink = Flink;\
		(Entry)->Blink = pHead;\
		Flink->Blink = (Entry);\
		pHead->Flink = (Entry);\
	}


#endif