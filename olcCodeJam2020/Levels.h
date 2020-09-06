#pragma once
#include "olcPixelGameEngine.h"

struct Machine { // Player
	float x;
	float y;
	float vx;
	float vy;
	float initX, initY;
	float angleBody;
	float angleTurret;

	std::vector<olc::vf2d> vPath;
	float fSpeed;
	int nHealth;
	bool bDead = false;
};

struct Barrier {
	olc::vf2d pos;
	olc::vf2d size;
	bool bHarmful;
	int nInteractDamage;
	Barrier(float x, float y, float wx, float wy, bool bHarm, int nDamage)
	{
		pos = olc::vf2d(x, y);
		size = olc::vf2d(wx, wy);
		bHarmful = bHarm;
		nInteractDamage = nDamage;
	}
};

struct Exit {
	olc::vf2d pos;
	olc::vf2d size;
	bool bGated;
	bool bGateOpen = false; // Gate starts closed
	Exit(float x, float y, float wx, float wy, bool bGate)
	{
		pos = olc::vf2d(x, y);
		size = olc::vf2d(wx, wy);
		bGated = bGate;
	}
};

class LevelLayout {
public:
	std::vector<Barrier*> barriers;
	//std::vector<Enemy*> enemies;
	//std::vector<Item*> items;
	Exit* exit;
	float fPlayerInitX, fPlayerInitY;
public:
	virtual void Create(olc::PixelGameEngine* pge) = 0;
};

class Level1 : public LevelLayout
{
	void Create(olc::PixelGameEngine* pge) override;
};

void Level1::Create(olc::PixelGameEngine* pge)
{
	fPlayerInitX = 50.0f;
	fPlayerInitY = 60.0f;

	barriers.push_back(new Barrier(pge->ScreenWidth() / 2, 80, 30, 60, false, 0));

	exit = new Exit(pge->ScreenWidth() - 30, 0, 30, pge->ScreenHeight(), false);
}

class Level2 : public LevelLayout
{
	void Create(olc::PixelGameEngine* pge) override;
};

void Level2::Create(olc::PixelGameEngine* pge)
{
	fPlayerInitX = 110.0f;
	fPlayerInitY = 90.0f;

	barriers.push_back(new Barrier(pge->ScreenWidth() / 6, 60, 30, 400, false, 0));
	barriers.push_back(new Barrier(pge->ScreenWidth() - 100, 0, 30, 100, false, 0));


	exit = new Exit(pge->ScreenWidth() - 30, 0, 30, 50, false);
}