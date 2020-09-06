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

struct Projectile {
	float x, y, vx, vy;
	float fSpeed;
	bool bActive;
	bool bShot;
};

struct Barrier {
	olc::vf2d pos;
	olc::vf2d size;
	bool bHarmful;
	int nInteractDamage;
	bool bActive = true;
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
	fPlayerInitX = 40.0f;
	fPlayerInitY = 240.0f;

	barriers.push_back(new Barrier(pge->ScreenWidth() / 7, 70, 50, 340, false, 0));


	exit = new Exit(pge->ScreenWidth() - 30, 0, 30, pge->ScreenHeight(), false);
}

class Level2 : public LevelLayout
{
	void Create(olc::PixelGameEngine* pge) override;
};

void Level2::Create(olc::PixelGameEngine* pge)
{
	fPlayerInitX = 70.0f;
	fPlayerInitY = 100.0f;

	barriers.push_back(new Barrier(pge->ScreenWidth() / 6, 60, 30, 400, false, 0));
	barriers.push_back(new Barrier(pge->ScreenWidth() - 100, 0, 30, 100, false, 0));


	exit = new Exit(pge->ScreenWidth() - 30, 0, 30, 50, false);
}

class Level3 : public LevelLayout
{
	void Create(olc::PixelGameEngine* pge) override;
};

void Level3::Create(olc::PixelGameEngine* pge)
{
	fPlayerInitX = 70.0f;
	fPlayerInitY = 100.0f;

	barriers.push_back(new Barrier(pge->ScreenWidth() / 2, 398, 250, 60, false, 0));
	barriers.push_back(new Barrier(pge->ScreenWidth() / 6, 0, 30, 390, false, 0));
	barriers.push_back(new Barrier(pge->ScreenWidth() - 100, 0, 30, 100, false, 0));
	barriers.push_back(new Barrier(pge->ScreenWidth() - 90, 120, 60, 10, false, 0));
	barriers.push_back(new Barrier(pge->ScreenWidth() - 400, 30, 150, 90, false, 0));




	exit = new Exit(pge->ScreenWidth() - 30, 0, 30, 50, false);
}

class Level4 : public LevelLayout
{
	void Create(olc::PixelGameEngine* pge) override;
};

void Level4::Create(olc::PixelGameEngine* pge)
{
	fPlayerInitX = 700.0f;
	fPlayerInitY = 390.0f;

	barriers.push_back(new Barrier(1, 1, 250, 20, false, 0));
	barriers.push_back(new Barrier(1, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(100, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(200, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(300, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(400, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(500, 1, 30, 90, false, 0));

	barriers.push_back(new Barrier(pge->ScreenWidth() - 400, 230, 300, 20, false, 0));
	//barriers.push_back(new Barrier(pge->ScreenWidth() - 100, 0, 30, 100, false, 0));
	//barriers.push_back(new Barrier(pge->ScreenWidth() - 90, 120, 60, 10, false, 0));
	//barriers.push_back(new Barrier(pge->ScreenWidth() - 400, 30, 150, 90, false, 0));




	exit = new Exit(1, 30, 40, 40, false);
}

class Level5 : public LevelLayout
{
	void Create(olc::PixelGameEngine* pge) override;
};

void Level5::Create(olc::PixelGameEngine* pge)
{
	fPlayerInitX = 30.0f;
	fPlayerInitY = 50.0f;

	barriers.push_back(new Barrier(80, 200, 450, 20, false, 0));
	barriers.push_back(new Barrier(1, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(100, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(200, 120, 50, 20, false, 0));
	//barriers.push_back(new Barrier(300, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(400, 120, 50, 20, false, 0));
	barriers.push_back(new Barrier(500, 1, 30, 90, false, 0));

	barriers.push_back(new Barrier(pge->ScreenWidth() - 300, pge->ScreenHeight() / 4 + 60, 400, 50, false, 0));
	barriers.push_back(new Barrier(pge->ScreenWidth() - 200, pge->ScreenHeight() / 4 - 80, 200, 50, false, 0));

	//barriers.push_back(new Barrier(pge->ScreenWidth() - 100, 0, 30, 100, false, 0));
	//barriers.push_back(new Barrier(pge->ScreenWidth() - 90, 120, 60, 10, false, 0));
	//barriers.push_back(new Barrier(pge->ScreenWidth() - 400, 30, 150, 90, false, 0));




	exit = new Exit(pge->ScreenWidth() - 30, pge->ScreenHeight() / 4, 30, 50, false);
}