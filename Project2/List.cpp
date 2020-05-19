#include "List.h"
#include "Map.h"

LinkList::LinkList()
{
	T = nullptr;
	H = nullptr;
}

void LinkList::AddElement()
{
	List* ptr = new List;
	if (H == nullptr && T == nullptr)
	{
		H = ptr;
		T = ptr;
		ptr->next = ptr;
		ptr->prev = ptr;
	}
	else
	{
		T->next = ptr;
		ptr->prev = T;
		T = ptr;
		T->next = H;
		H->prev = T;
	}
}

void LinkList::DelElement(int code)
{
	if (H != nullptr)
	{
		List* ptr = T;
		while (true)
		{
			if (ptr == H)
			{
				H = H->next;
				ptr->prev->next = ptr->next;
				ptr->next->prev = ptr->prev;
				delete ptr;
				break;
			}
			else if (ptr == T)
			{
				T = T->prev;
				ptr->prev->next = ptr->next;
				ptr->next->prev = ptr->prev;
				delete ptr;
			}
			ptr = ptr->prev;
		}
	}
}


void LinkList::ClearList()
{
	List* ptr = T;
	while (H != nullptr)
	{
		if (ptr != H)
		{
			T = T->prev;
			T->next = H;
			delete ptr;
			ptr = T;
		}
		else if(ptr == H)
		{
			T = nullptr;
			H = nullptr;
			delete ptr;
		}
	}
}