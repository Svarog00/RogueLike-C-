#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <chrono>
#include "Map.h"
#include "Entities.h"
#include "List.h"

void Setting()
{
	HWND hwnd = GetConsoleWindow(); //получение дескриптора окна
	//LPCWSTR = Long Pointer on Constant wchar
	LPCWSTR title = L"Rogue_by_Svarog00";
	SetConsoleTitle(title);
	MoveWindow(hwnd, 0, 0, 1000, 800, TRUE);//xnew,ynew,wnew,hnew -новые положение x,y, ширина и высота
}

void ShowResults(int tKills, int tMaps)
{
	std::cout << "Total kills: " << tKills << std::endl
		<< "Completed maps: " << tMaps << std::endl;
}

void Game(int& tKills, int& tMaps)
{
	srand((unsigned int)time(0));
	bool turn = false; //can enemy attack?
	bool play = true;

	LinkList theList;
	List* curMap;
	Player* player = new Player(10, 2, 2);

	theList.AddElement();
	theList.T->MapPack = Map();
	curMap = theList.T;

	while (play)
	{
		player->Action();
		curMap->MapPack.DrawMap(player, turn);
		if (curMap->MapPack.IsCompleted() == true)
		{
			tKills += player->GetKills();
			tMaps += 1;
			theList.AddElement();
			theList.T->MapPack = Map();
			player->ZeroKills(); //set player kills to zero
		}
		if (curMap->MapPack.IsNext())
		{
			curMap = curMap->next;
			player->SetCoord(2, 2);
			curMap->MapPack.ZeroNext();
		}
		if (curMap->MapPack.IsPrev())
		{
			curMap = curMap->prev;
			player->SetCoord(2, 2);
			curMap->MapPack.ZeroPrev();
		}
		if (curMap->MapPack.IsFailed() == true)
		{
			theList.ClearList();
			play = false;
		}

		if (turn) //change turn
			turn = false;
		else
			turn = true;

		Sleep(20);
		system("cls");
	}
	system("cls");
	ShowResults(tKills, tMaps);
	tKills = 0; tMaps = 0;
}



int main()
{
	int totalKills = 0, completedMaps = 0;
	Setting();
	while (true)
	{
		Game(totalKills, completedMaps);
		std::cout << "New game?\n Y - Yes\n N - No\n";
		char key = _getch();
		if (key == 'y' || key == 'Y')
		{
			continue;
		}
		else if (key == 'n' || key == 'N')
		{
			break;
		}
		else
		{

		}
	}

	_getch();
	return 0;
}