#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Entities.h"
#include "UtilityFunctions.h"

void Entity::Hurt(int dmg)
{
	_health -= dmg;
}

int Entity::GetX()
{
	return X;
}

int Entity::GetY()
{
	return Y;
}

void Entity::SetCoord(int nx, int ny)
{
	X = nx; Y = ny;
}

int Entity::GetHealth() const
{
	return _health;
}

void Entity::Deactivate()
{
	isActive = false;
}

bool Entity::IsActivated()
{
	return isActive;
}


//Player
Player::Player(int Health, int x, int y)
{
	_health = Health;
	X = x; Y = y;
	kills = 0;
	completedMaps = 0;
}

int Player::GetDir()
{
	return dir;
}

void Player::Action()
{
	static char key;
	if (_kbhit())
	{
		key = _getch();
		if (key == 'w' || key == 'W')
		{
			dir = North;
			Y--;
		}
		else if (key == 's' || key == 'S')
		{
			dir = South;
			Y++;
		}
		else if (key == 'a' || key == 'A')
		{
			dir = West;
			X--;
		}
		else if (key == 'd' || key == 'D')
		{
			dir = East;
			X++;
		}
		if (key == 'e' || key == 'E')
		{
			_isAttack = true;
		}
	}
}

void Player::Show(HDC *hdc) const
{
	HBRUSH brush = CreateHatchBrush(7, RGB(100, 255, 38)); //создание кисти
	SelectObject(*hdc, brush); //выбор кисти и дескриптора
	Rectangle(*hdc, X * 15, Y * 15, X * 15+10, Y * 15+10); //рисование прямоугольника с помощью выбранного дескриптора
	DeleteObject(brush); //удаление кисти
}

bool Player::Attack()
{
	return _isAttack;
}
void Player::CancelAttack()
{
	_isAttack = false;
}

//kills and vampirism
int Player::GetKills() const
{
	return kills;
}

void Player::ZeroKills()
{
	kills = 0;
}

void Player::UpKills()
{
	kills += 1;
	Vampire();
}

void Player::Vampire()
{
	if(_health < 10)
		_health+=4;
}
//------------------------
//Swashbuckler
Swashbuckler::Swashbuckler()
{
	_health = 2;
}

Swashbuckler::Swashbuckler(int x, int y)
{
	_health = 2;
	X = x; Y = y;
}

void Swashbuckler::Show(HDC *hdc) const
{
	HBRUSH brush = CreateHatchBrush(7, color); //создание кисти определенного размера и цвета
	SelectObject(*hdc, brush); //выбираем эту кисть
	Rectangle(*hdc, X * 15, Y * 15, X * 15 + 10, Y * 15 + 10); //рисуем прямоугольник
	DeleteObject(brush); //удаляем кисть
}

void Swashbuckler::Action(Entity* pl, bool turn)
{
	if((Distance(pl->GetX(), pl->GetY(), X, Y) > 1) && (Distance(pl->GetX(), pl->GetY(), X, Y) < 4) && isActive == true)
	{
		if (pl->GetX() < X)
		{
			if (pl->GetY() > Y)
			{
				X--;
				Y++;
			}
			else if(pl->GetY() < Y)
			{
				X--;
				Y--;
			}

		}
		else if (pl->GetX() > X)
		{
			if (pl->GetY() < Y)
			{
				X++;
				Y--;
			}
			if(pl->GetY() > Y)
			{
				X++;
				Y++;
			}
		}
		if (pl->GetX() == X)
		{
			if (pl->GetY() > Y)
				Y++;
			if (pl->GetY() < Y)
				Y--;
		}
		if (pl->GetY() == Y)
		{
			if (pl->GetX() > X)
			{
				X++;
			}

			if (pl->GetX() < X)
			{
				X--;
			}
				
		}
	}
	if ((Distance(pl->GetX(), pl->GetY(), X, Y) <= 1) && (isActive == true) && (turn == true))
	{
		Attack(pl);
	}

	if (isActive == false)
	{
		color = RGB(255, 255, 255);
	}
}
bool Swashbuckler::Attack(Entity* pl)
{
	pl->Hurt(1);
	return false;
}

Ranger::Ranger()
{
	_health = 1;
}

Ranger::Ranger(int x, int y)
{
	_health = 1;
	X = x, Y = y;
	color = RGB(216, 105, 30);
	shot = Shot(x, y);
}

void Ranger::Show(HDC* hDC) const
{
	HBRUSH brush = CreateHatchBrush(7, color); //создание кисти определенного размера и цвета
	SelectObject(*hDC, brush); //выбираем эту кисть
	Rectangle(*hDC, X * 15, Y * 15, X * 15 + 10, Y * 15 + 10); //рисуем прямоугольник
	DeleteObject(brush); //удаляем кисть
	
	shot.Show(hDC);
}

void Ranger::Action(Entity* pl, bool turn)
{
	if ((Distance(pl->GetX(), pl->GetY(), X, Y) > 1) && (isActive == true) && (turn == true))
	{
		Attack(pl);
	}

	if (isActive == false)
	{
		color = RGB(255, 255, 255);
	}
}

bool Ranger::Attack(Entity* pl)
{
	if (!shot.isExisting)
	{
		shot.SetCoord(X, Y);
		shot.isExisting = true;
	}
	shot.Move(pl);
	return false;
}

Shot::Shot()
{
	X = 0; Y = 0;
	isExisting = false;
}

Shot::Shot(int x, int y)
{
	X = x; Y = y;
	isExisting = false;
}

void Shot::Show(HDC* hDC) const
{
	if (isExisting)
	{
		HBRUSH brush = CreateHatchBrush(7, RGB(0, 255, 255)); //создание кисти
		SelectObject(*hDC, brush); //выбор кисти и дескриптора
		Rectangle(*hDC, X * 15, Y * 15, X * 15 + 10, Y * 15 + 10); //рисование прямоугольника с помощью выбранного дескриптора
		DeleteObject(brush); //удаление кисти
	}	
}

void Shot::Move( Entity* pl)
{
	if ((Distance(pl->GetX(), pl->GetY(), X, Y) > 1) && isExisting == true)
	{
		if (pl->GetX() < X)
		{
			if (pl->GetY() > Y)
			{
				X--;
				Y++;
			}
			else if (pl->GetY() < Y)
			{
				X--;
				Y--;
			}

		}
		else if (pl->GetX() > X)
		{
			if (pl->GetY() < Y)
			{
				X++;
				Y--;
			}
			if (pl->GetY() > Y)
			{
				X++;
				Y++;
			}
		}
		if (pl->GetX() == X)
		{
			if (pl->GetY() > Y)
				Y++;
			if (pl->GetY() < Y)
				Y--;
		}
		if (pl->GetY() == Y)
		{
			if (pl->GetX() > X)
			{
				X++;
			}

			if (pl->GetX() < X)
			{
				X--;
			}

		}
	}

	if (X == pl->GetX() && Y == pl->GetY())
	{
		pl->Hurt(1);
		isExisting = false;
	}
}

void Shot::SetCoord(int nx, int ny)
{
	X = nx, Y = ny;
}


