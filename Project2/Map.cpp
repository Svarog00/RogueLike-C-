#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <conio.h>
#include "Map.h"
#include "Entities.h"


Map::Map()
{
	int tx, ty;
	m = rand() % 30 + 5;
	n = rand() % 40 + 5;
	while (n == 0 || m == 0)//If any of border is zero - reroll them
	{
		m = rand() % 30 + 5;
		n = rand() % 40 + 5;
	}

	map = new int*[m]; //creating dyn array for map
	for (unsigned int i = 0; i < m; i++)
	{
		map[i] = new int[n];
	}

	c = rand() % 5 + 1; //random number of enemies
	c++; //and one is ranger
	enemies = new Entity*[c];

	for (unsigned int i = 0; i < c-1; i++)
	{
		tx = rand() % n;
		ty = rand() % m;
		if (tx == n && ty == m)
		{
			tx = rand() % n;
			ty = rand() % m;
		}
		enemies[i] = new Swashbuckler(tx, ty);
	}

	//generate the ranger in array
	tx = rand() % n;
	ty = rand() % m;
	if (tx == n && ty == m)
	{
		tx = rand() % n;
		ty = rand() % m;
	}
	enemies[c-1] = new Ranger(tx, ty);
	//generating over

	FillMap();
	completed = false;

	prev = false;
	next = false;
}

void Map::ShowMap(Player* player)
{
	HDC hDCT = CreateCompatibleDC(hDCmap); //дескриптор, через который будет происходить рисование в окне
	HBITMAP bmpMem = CreateCompatibleBitmap(hDCmap, WinCoord.right, WinCoord.bottom); //дескриптор точечного рисунка, который будет выведен в статическом элементе 
	SelectObject(hDCT, bmpMem); //выбор дескриптора точечного рискунка
	BitBlt(hDCT, 0, 0, WinCoord.right, WinCoord.bottom, hDCT, 0, 0, SRCCOPY); //выполняет передачу битовых блоков данных о цвете, соответствующих прямоугольнику пикселей из заданного исходного контекста устройства в целевой контекст устройства.
	//SRCCOPY Копирует исходный прямоугольник непосредственно в целевой прямоугольник.
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); //Создаём кисть определённого стиля и цвета
	SelectObject(hDCT, brush); //Выбираем кисть
	Rectangle(hDCT, 0, 0, WinCoord.right, WinCoord.bottom); //Рисуем фон
	DeleteObject(brush);

	//Logic and drawing of everything
	for (unsigned int i = 0; i < m; i++)
	{
		for (unsigned int j = 0; j < n; j++)
		{
			if (player->GetX() == n - 1 && player->GetY() == m / 2)
			{
				next = true;
			}
			else if (player->GetX() == 0 && player->GetY() == m / 2)
			{
				prev = true;
			}

			//borders of map
			if (player->GetX() == n - 1)
				player->SetCoord(player->GetX() - 1, player->GetY());
			else if(player->GetX() == 0)
				player->SetCoord(player->GetX() + 1, player->GetY());
			else if (player->GetY() == m - 1)
				player->SetCoord(player->GetX(), player->GetY()-1);
			else if (player->GetY() == 0)
				player->SetCoord(player->GetX(), player->GetY() + 1);

			//collision with enemies
			for (unsigned int k = 0; k < c; k++)
			{
				if (player->GetDir() == 0)
				{
					if ((enemies[k]->GetX() == player->GetX()) && (enemies[k]->GetY() == player->GetY()))
					{
						player->SetCoord(player->GetX(), player->GetY() - 1);
					}
				}
				else if (player->GetDir() == 1)
				{
					if ((enemies[k]->GetX() == player->GetX()) && (enemies[k]->GetY() == player->GetY()))
					{
						player->SetCoord(player->GetX(), player->GetY() + 1);
					}
				}
				else if (player->GetDir() == 2)
				{
					if ((enemies[k]->GetX() == player->GetX()) && (enemies[k]->GetY() == player->GetY()))
					{
						player->SetCoord(player->GetX()-1, player->GetY());
					}
				}
				else if (player->GetDir() == 3)
				{
					if ((enemies[k]->GetX() == player->GetX()) && (enemies[k]->GetY() == player->GetY()))
					{
						player->SetCoord(player->GetX()+1, player->GetY());
					}
				}
					
			}

			if (player->Attack() == true) //player attack
			{
				for (unsigned int k = 0; k < c; k++)
				{
					//if enemy is nearby
					if ( (enemies[k]->GetX() == player->GetX() - 1) && (enemies[k]->GetY() == player->GetY()) ||
						(enemies[k]->GetX() == player->GetX() + 1) && (enemies[k]->GetY() == player->GetY()) ||
						(enemies[k]->GetX() == player->GetX()) && (enemies[k]->GetY() == player->GetY() - 1) ||
						(enemies[k]->GetX() == player->GetX()) && (enemies[k]->GetY() == player->GetY() + 1))
					{
						int a = k;
						enemies[a]->Hurt(1); //the nearest enemy is hurting by the player
						player->CancelAttack();
						if (enemies[a]->GetHealth() == 0)
						{
							enemies[a]->Deactivate(); //deactive the enemy
							player->UpKills(); //add kill in feed
						}
					}
					else
					{
						player->CancelAttack();//if there is noone here then just cancel attack
					}
				}
			}
			//-----------------------------------------------------
			//drawing and logic of enemies
			for (unsigned int k = 0; k < c; k++)
			{
				if (enemies[k]->GetX() == j && enemies[k]->GetY() == i) 
				{
					enemies[k]->Action(player, canAttack); //the enemy do the thing
					enemies[k]->Show(&hDCT); //draw the enemy
					j++;
				}
			}

			//collision between enemies
			for (unsigned int k = 0; k < c; k++)
			{
				for(unsigned int j = 0; j < c; j++)
				{
					if (j == k)
					{
						continue;
					}
					else if ((enemies[k]->GetX() == enemies[j]->GetX()) && (enemies[k]->GetY() == enemies[j]->GetY()))
					{
						enemies[k]->SetCoord(enemies[k]->GetX(), enemies[k]->GetY());
					}
				}

			}

			//borders depend on health of player
			int Color = RGB(0, 0, 255);
			if(player->GetHealth() >= 9)
			{
				Color = RGB(0, 0, 255);
			}
			else if (player->GetHealth() < 9 && player->GetHealth() > 5)
			{
				Color = RGB(100, 0, 150);
			}
			else if (player->GetHealth() <= 5 && player->GetHealth() > 2)
			{
				Color = RGB(100, 0, 100);
			}
			else if(player->GetHealth() <= 2)
			{
				Color = RGB(150, 0, 50);
			}
			//============================================

			//draw environment
			if (map[i][j] == 0) //borders
			{
				HBRUSH brush = CreateHatchBrush(7, Color);
				SelectObject(hDCT, brush);
				Rectangle(hDCT, j * 15, i *15, j * 15 + 10, i * 15 + 10);
				DeleteObject(brush);
			}
			else if (map[i][j] == 1) //floor
			{
				HBRUSH brush = CreateHatchBrush(7, RGB(0, 0, 0));
				SelectObject(hDCT, brush);
				Rectangle(hDCT, j * 15, i * 15, j * 15 + 10, i * 15 + 10);
				DeleteObject(brush);
			}
			//--------------------------------------------------------------
			//player section--------------------------------------
			if (player->GetX() == j && player->GetY() == i)
			{
				player->Show(&hDCT);
			}

			if (player->GetHealth() == 0)//if the player is dead then the map is failed and the game is over
			{
				failed = true; 
			}

			if ((player->GetKills() == c)) //if the player killed all of the enemies then map is completed
			{
				map[m / 2][n-1] = 1;
				completed = true;
			}

		}
		std::cout << std::endl;
	}

	long retval = BitBlt(hDCmap, 0, 0, WinCoord.right - WinCoord.left, WinCoord.bottom - WinCoord.top, hDCT, 0, 0, SRCCOPY);
	
	DeleteDC(hDCT);
	
	//The InvalidateRect function adds a rectangle to the specified window's update region
}

void Map::FillMap()
{
	//0 - border
	//1 - floor
	//2 - door
	for (unsigned int i = 0; i < m; i++)
	{
		for (unsigned int j = 0; j < n; j++)
		{
			if (i == m / 2 && j == 0)
				map[i][j] = 1;

			else if (i == 0 && j >= 0 || i != 0 && j == 0)
				map[i][j] = 0;

			else if (j == n - 1 && i > 0)
				map[i][j] = 0;

			else if (i == m - 1 && j > 0)
				map[i][j] = 0;

			else
				map[i][j] = 1;

		}
	}
}

bool Map::IsCompleted()
{
	return completed;
}

bool Map::IsFailed()
{
	return failed;
}

bool Map::IsNext()
{
	return next;
}

bool Map::IsPrev()
{
	return prev;
}

void Map::ZeroNext()
{
	next = false;
}

void Map::ZeroPrev()
{
	prev = false;
}

void Map::DrawMap(Player* player, bool pass)
{
	canAttack = pass; //variable for the enemies to pass the turn
	GetWindowRect(GetConsoleWindow(), &WinCoord); //узнаем координаты окна
	hDCmap = GetDC(GetConsoleWindow());//создание дескриптора для рисования карты
	ShowMap(player);

}

