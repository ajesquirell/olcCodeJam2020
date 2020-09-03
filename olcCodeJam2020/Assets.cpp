#include "Assets.h"

using namespace std;

Assets::Assets()
{
}

Assets::~Assets()
{
}

void Assets::LoadSprites() //Single one time load of all sprite resources
{
	auto load = [&](string name, string filename)
	{
		mapSprites[name] = new olc::Sprite(filename);
	};

	//Tank
	load("Tank_Body", "../Sprites/Tank_Body.png");
	load("Tank_Turret", "../Sprites/Tank_Turret.png");
	load("Tank_Wheels", "../Sprites/Tank_Wheels.png");

}

//void Assets::LoadMaps()
//{
//	auto load = [&](cMap* map)
//	{
//		mapMaps[map->sName] = map;
//	};
//
//	load(new cMap_Level1);
//	load(new cMap_Level2);
//}

//void Assets::LoadItems()
//{
//	auto load = [&](cItem* item, int id = 0)
//	{
//		mapItems[item->sName] = id;
//	};
//
//	load(new cItem_Health, 1);
//	load(new cItem_HealthBoost, 2);
//	load(new cItem_FlamesCash, 3);
//	load(new cWeapon_Sword, 4);
//
//}

void Assets::LoadSounds()
{
	auto load = [&](string name, string filename)
	{
		//mapSounds[name] = olc::SOUND::LoadAudioSample(filename);
	};

	load("sndSampleA", "../Sounds/SampleA.wav");
}

