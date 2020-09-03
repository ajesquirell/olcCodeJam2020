#pragma once

#include <iostream>
#include <map>

#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"


class Assets
{
public:
	static Assets& get() //"static" makes it a method applied directly to class, don't need to create an instance to use
	{
		static Assets me;
		return me;
	}

	//Ensure use as singleton
	Assets(Assets const&) = delete; //Delete default copy constructor
	void operator=(Assets const&) = delete; //Make sure the one global instance can't be duplicated


	olc::Sprite* GetSprite(std::string name)
	{
		if (mapSprites[name] == nullptr)
			std::cerr << "\nError: Could not retrieve sprite with the name \"" << name << "\". Please ensure it exists on disk and is loaded from Assets class.\n";

		return mapSprites[name]; //If "name" is specified wrongly or doesn't exist, it will just return a blank sprite
	}

	int GetSound(std::string name)
	{
		if (mapSounds[name] == 0)
			std::cerr << "\nError: Could not retrieve sound with the name \"" << name << "\". Please ensure it exists on disk and is loaded from Assets class.\n";

		return mapSounds[name];
	}

	void LoadSprites();
	//void LoadMaps();
	//void LoadItems();
	void LoadSounds();

private:
	Assets();
	~Assets();

	std::map<std::string, olc::Sprite*> mapSprites;
	std::map<std::string, int> mapItems;
	std::map<std::string, int> mapSounds;
};

