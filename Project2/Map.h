#pragma once
#include "Entities.h"
#include <string>

class Map
{
private:
	int** map;
	unsigned int n, m; //borders
	unsigned int c; //number of enemies

	bool canAttack;

	int id;

	bool completed;
	bool failed;
	bool next;
	bool prev;

	Entity** enemies;
	RECT WinCoord = {};
	HDC hDCmap;
public:
	Map();
	void ShowMap(Player* player);
	void DrawMap(Player* player, bool pass);
	void FillMap();

	bool IsCompleted();
	bool IsFailed();

	void ZeroPrev();
	void ZeroNext();

	bool IsNext();
	bool IsPrev();

};

