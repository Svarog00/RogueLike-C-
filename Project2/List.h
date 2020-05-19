#pragma once
#pragma once

#include "Map.h"

struct List
{
	Map MapPack;
	List* prev;
	List* next;
};


class LinkList
{
public:
	List* H;
	List* T;
public:
	LinkList();
	void AddElement();
	void DelElement(int code);
	void ClearList();
};

