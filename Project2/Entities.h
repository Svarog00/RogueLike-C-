#pragma once
#ifndef ENTITIES_H
#define ENTITIES_H
#include <Windows.h>
#include "UtilityFunctions.h"

enum Direction { North, South, West, East };

class Entity
{
protected:
	int X, Y;
	int _health;
	int _dmg = 3;
	bool isActive = true;
public:
	virtual ~Entity();
	
	virtual void Show(HDC* hdc) const {}
	virtual void Action(Entity* pl, bool turn) {}
	virtual bool Attack() { return false; }

	void Hurt(int dmg);

	int GetX();
	int GetY();

	int GetHealth() const; 
	bool IsActivated();

	void Deactivate();
	void SetCoord(int nx, int ny);
};

class Player : public Entity
{
private:
	bool _isAttack = false;
	Direction dir = North;
	int kills;
	int completedMaps;
public:
	Player(int Health, int x, int y);
	~Player() override;
	virtual void Show(HDC* hdc) const;
	virtual void Action();
	virtual bool Attack();

	int GetDir();
	void CancelAttack();

	void Vampire();

	int GetKills() const;
	void ZeroKills();
	void UpKills();
};

class Swashbuckler : public Entity
{
protected:
	int color = RGB(255, 0, 0);
public:
	Swashbuckler();
	Swashbuckler(int x, int y);
	~Swashbuckler() override;
	virtual void Show(HDC *hDC) const;
	virtual void Action(Entity* pl, bool turn);
	virtual bool Attack(Entity* pl);
};

class Shot
{
private:
	int X, Y;
public:
	bool isExisting;
	Shot();
	Shot(int x, int y);
	void Show(HDC* hDC) const;
	void Move(Entity* pl);
	void SetCoord(int nx, int ny);
};

class Ranger : public Swashbuckler
{
private:
	Shot shot;
public:
	Ranger();
	Ranger(int x, int y);
	~Ranger() override;
	virtual void Show(HDC* hDC) const;
	virtual void Action(Entity* pl, bool turn);
	virtual bool Attack(Entity* pl);
};

#endif