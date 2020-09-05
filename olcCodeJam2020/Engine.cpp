#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"
#define OLC_PGEX_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"
#include "Assets.h"

#define pi 3.14159f

class MvM : public olc::PixelGameEngine
{
public:
	MvM()
	{
		sAppName = "Machine v. Machine";
	}

private:
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

	struct LevelLayout {
		std::vector<Barrier*> barriers;
		//std::vector<Enemy*> enemies;
		//std::vector<Item*> items;
		Exit* exit;
		float fPlayerInitX, fPlayerInitY;

	};
	
	std::vector<LevelLayout> levels;
	LevelLayout currentLevel;

	Machine* player;

	std::unique_ptr<olc::Sprite> sprBody;
	std::unique_ptr<olc::Decal> decBody;
	std::unique_ptr<olc::Sprite> sprTurret;
	std::unique_ptr<olc::Decal> decTurret;
	std::unique_ptr<olc::Sprite> sprWheels;
	std::unique_ptr<olc::Decal> decWheels;

	std::unique_ptr<olc::Sprite> sprFire1;
	std::unique_ptr<olc::Sprite> sprFire2;
	std::unique_ptr<olc::Sprite> sprFire3;
	std::unique_ptr<olc::Sprite> sprFire4;
	std::unique_ptr<olc::Sprite> sprFire5;
	std::unique_ptr<olc::Sprite> sprFire6;
	std::unique_ptr<olc::Sprite> sprFire7;
	std::unique_ptr<olc::Sprite> sprFire8;
	std::unique_ptr<olc::Decal> decFire1;
	std::unique_ptr<olc::Decal> decFire2;
	std::unique_ptr<olc::Decal> decFire3;
	std::unique_ptr<olc::Decal> decFire4;
	std::unique_ptr<olc::Decal> decFire5;
	std::unique_ptr<olc::Decal> decFire6;
	std::unique_ptr<olc::Decal> decFire7;
	std::unique_ptr<olc::Decal> decFire8;


	//Barriers
	std::vector<Barrier*> barriers;

	// Exit
	Exit* exit;
	olc::Sprite* tempExitSign;

	olc::vi2d tileSize = olc::vi2d(1, 1);
	olc::vi2d playerSize = olc::vi2d(16, 16);

	// Path and point Variables
	bool run = false;
	int pathCnt = 1;
	float fTimeSoFar = 0;
	float fDuration = 3;
	int pointRadius = 2;

	float timeCursor; // For little animations
	float timeCursor2;
	float timeFire;

	// Scores
	int nCurrentLevel;
	int nLevelAttempts;

public:
	bool OnUserCreate() override
	{
		player = new Machine();
		player->initX = 50.0f;
		player->initY = 60.0f;
		player->fSpeed = 20.0f;

		player->x = player->initX;
		player->y = player->initY;
		player->vx = 0.0f;
		player->vy = 0.0f;
		player->angleBody = 0.0f;
		player->nHealth = 2;


		player->vPath.push_back(olc::vf2d(player->x, player->y));

		barriers.push_back(new Barrier(ScreenWidth() - 80, 20, 30, 90, false, 0));

		exit = new Exit(ScreenWidth() - 30, 0, 30, ScreenHeight(), false);
		tempExitSign = new olc::Sprite(35, 8);
		SetDrawTarget(tempExitSign);
		Clear(olc::DARK_CYAN);
		DrawString({ 0, 0 }, "EXIT", olc::VERY_DARK_CYAN);
		SetDrawTarget(nullptr);

		

		Assets::get().LoadSprites();

		// Load Player Sprites
		sprBody = std::make_unique<olc::Sprite>("../Sprites/Tank_Body.png");
		sprTurret = std::make_unique<olc::Sprite>("../Sprites/Tank_Turret.png");
		sprWheels = std::make_unique<olc::Sprite>("../Sprites/Tank_Wheels.png");

		// Create Decals
		decBody = std::make_unique<olc::Decal>(sprBody.get());
		decTurret = std::make_unique<olc::Decal>(sprTurret.get());
		decWheels = std::make_unique<olc::Decal>(sprWheels.get());

		// Fire Sprites and Decals
		sprFire1 = std::make_unique<olc::Sprite>("../Sprites/fire/1.png");
		sprFire2 = std::make_unique<olc::Sprite>("../Sprites/fire/2.png");
		sprFire3 = std::make_unique<olc::Sprite>("../Sprites/fire/3.png");
		sprFire4 = std::make_unique<olc::Sprite>("../Sprites/fire/4.png");
		sprFire5 = std::make_unique<olc::Sprite>("../Sprites/fire/5.png");
		sprFire6 = std::make_unique<olc::Sprite>("../Sprites/fire/6.png");
		sprFire7 = std::make_unique<olc::Sprite>("../Sprites/fire/7.png");
		sprFire8 = std::make_unique<olc::Sprite>("../Sprites/fire/8.png");
		decFire1 = std::make_unique<olc::Decal>(sprFire1.get());
		decFire2 = std::make_unique<olc::Decal>(sprFire2.get());
		decFire3 = std::make_unique<olc::Decal>(sprFire3.get());
		decFire4 = std::make_unique<olc::Decal>(sprFire4.get());
		decFire5 = std::make_unique<olc::Decal>(sprFire5.get());
		decFire6 = std::make_unique<olc::Decal>(sprFire6.get());
		decFire7 = std::make_unique<olc::Decal>(sprFire7.get());
		decFire8 = std::make_unique<olc::Decal>(sprFire8.get());



		//Sound
		//olc::SOUND::InitialiseAudio(44100, 1, 8, 512);
		//olc::SOUND::PlaySample(Assets::get().GetSound("LitLoop"), true); // Plays Sample C loop

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::VERY_DARK_RED);

		//player->vx = 0.0f;
		//player->vy = 0.0f;

		if (IsFocused())
		{
			if (GetKey(olc::Key::UP).bHeld)
			{
				player->vy = -player->fSpeed;
			}

			if (GetKey(olc::Key::DOWN).bHeld)
			{
				player->vy = player->fSpeed;
			}

			if (GetKey(olc::Key::LEFT).bHeld)
			{
				player->angleBody -= 0.5f * fElapsedTime;
			}

			if (GetKey(olc::Key::RIGHT).bHeld)
			{
				player->angleBody += 0.5f * fElapsedTime;
			}

			if (GetMouse(0).bReleased && !run)
			{
				player->vPath.push_back(olc::vf2d(GetMouseX(), GetMouseY()));
			}

			if (GetMouse(1).bReleased && !run) // Right click to delete points, but only if not running. Otherwise vector can be out of range if currently going to point we delete
			{
				if (player->vPath.size() > 1)
					player->vPath.pop_back();
			}

			if (GetKey(olc::Key::SPACE).bReleased)
			{
				if (!run && player->vPath.size() > 1 && !player->bDead) // Only run if not already running
				{
					run = true;
					//player->x = initX;
					//player->y = initY;
				}
				if (player->bDead) // If player is dead
				{
					{
						ResetLevel();
						player->bDead = false;
						nLevelAttempts++;//Player try count++
					}
				}
			}
			if (GetKey(olc::Key::Z).bReleased)
			{
				player->bDead = !player->bDead;
			}
		}

		// Run path when ready
		if (run)
			PathUpdate(fElapsedTime, player);

		// Barrier collision detect
		for (int i = 0; i < barriers.size(); i++)
		{
			if (BarrierCollisionDetect(barriers[i]))
			{
				player->bDead = true;
			}
		}

		player->x += player->vx * fElapsedTime;
		player->y += player->vy * fElapsedTime;

		// Draw Barriers
		for (int i = 0; i < barriers.size(); i++)
		{
			FillRect(barriers[i]->pos, barriers[i]->size, olc::VERY_DARK_GREY);
			FillRect(barriers[i]->pos + olc::vf2d(2, 2), barriers[i]->size - olc::vf2d(4, 4), olc::GREY);
		}

		// Rotate and Draw Exit sign and Exit Bar
		FillRect(exit->pos, exit->size, olc::DARK_CYAN);

		olc::GFX2D::Transform2D tempExitTransform;
		tempExitTransform.Translate(-tempExitSign->width / 2.0f, -tempExitSign->height / 2.0f);
		tempExitTransform.Rotate(-(pi / 2));
		tempExitTransform.Translate(exit->pos.x + exit->size.x / 2, exit->pos.y + exit->size.y / 2 - 5);
		SetPixelMode(olc::Pixel::MASK);
		olc::GFX2D::DrawSprite(tempExitSign, tempExitTransform);
		SetPixelMode(olc::Pixel::NORMAL);
		

		// Draw Path Rays
		for (int i = 0; i < player->vPath.size() - 1; i++)
		{
			DrawLine(player->vPath[i], player->vPath[i + 1], olc::GREEN);
		}

		// Draw Path Points
		for (int i = 1; i < player->vPath.size(); i++)
		{
			FillCircle(player->vPath[i], pointRadius, olc::YELLOW);
		}

		if (!run) // When in planning mode, show possible point placement under mouse
		{
			// Line animation
			timeCursor2 += fElapsedTime;
			if (timeCursor2 > 0.6f) timeCursor2 = 0.0f;
			float t = timeCursor2 / 0.6f;
			
			//DrawLine(vPath.back(), olc::vi2d((GetMouseX() - vPath.back().x) * t + vPath.back().x, (GetMouseY() - vPath.back().y) * t + vPath.back().y), olc::WHITE, 0b10001000100010001000100010001000U);
			if (timeCursor2 >= 0.0f && timeCursor2 < 0.15f)
				DrawLine(player->vPath.back(), olc::vi2d(GetMouseX(), GetMouseY()), olc::MAGENTA, 0b10001000100010001000100010001000U);
			else if (timeCursor2 > 0.15f && timeCursor2 < 0.3f)
				DrawLine(player->vPath.back(), olc::vi2d(GetMouseX(), GetMouseY()), olc::MAGENTA, 0b01000100010001000100010001000100U);
			else if (timeCursor2 > 0.3f && timeCursor2 < 0.45f)
				DrawLine(player->vPath.back(), olc::vi2d(GetMouseX(), GetMouseY()), olc::MAGENTA, 0b00100010001000100010001000100010U);
			else
				DrawLine(player->vPath.back(), olc::vi2d(GetMouseX(), GetMouseY()), olc::MAGENTA, 0b00010001000100010001000100010001U);

			// Point animation
			timeCursor += fElapsedTime;
			if (timeCursor > 0.5f) timeCursor = 0.0f;
			if (timeCursor >= 0.0f && timeCursor < 0.08f)
			{
				DrawCircle(olc::vi2d(GetMouseX(), GetMouseY()), 6, olc::YELLOW);
				FillCircle(olc::vi2d(GetMouseX(), GetMouseY()), 2, olc::YELLOW);
			}
			else if (timeCursor > 0.08f && timeCursor < 0.16f)
			{
				DrawCircle(olc::vi2d(GetMouseX(), GetMouseY()), 4, olc::DARK_YELLOW);
				FillCircle(olc::vi2d(GetMouseX(), GetMouseY()), 2, olc::DARK_YELLOW);
			}
			else
				FillCircle(olc::vi2d(GetMouseX(), GetMouseY()), 2, olc::VERY_DARK_YELLOW);
		}


		// ===================================================Draw Player (drawing last so it is always on top)============================================
		//FillRect(olc::vf2d(player->x - (playerSize.x / 2), player->y - (playerSize.y / 2)), playerSize, olc::RED);
		olc::GFX2D::Transform2D tBody;
		olc::GFX2D::Transform2D tTurret;
		olc::GFX2D::Transform2D tWheels;

		// BODY and WHEELS
		tBody.Translate(-(float)Assets::get().GetSprite("Tank_Body")->width / 2.0f, -(float)Assets::get().GetSprite("Tank_Body")->height / 2.0f); // Translate to be able to rotate on center of sprite
		tWheels.Translate(-(float)Assets::get().GetSprite("Tank_Wheels")->width / 2.0f, -(float)Assets::get().GetSprite("Tank_Wheels")->height / 2.0f); // Translate to be able to rotate on center of sprite
		if (!player->bDead)
		{
			if (!run)
			{
				player->angleBody = pi / 2;
			}
			else
			{
				//player->angleBody = atan2(-(vPath[pathCnt].y - player->y), vPath[pathCnt].x - player->x);
				//player->angleBody = atan2(-player->vy, player->vx) - (pi/2);
				player->angleBody = atan2(player->vy, player->vx) + (pi / 2);
			}
			tBody.Rotate(player->angleBody);
			tBody.Scale(0.1, 0.1);
			tBody.Translate(player->x, player->y);

			tWheels.Rotate(player->angleBody);
			tWheels.Scale(0.1, 0.1);
			tWheels.Translate(player->x, player->y);

			// TURRET
			tTurret.Translate(-(float)Assets::get().GetSprite("Tank_Turret")->width / 2.0f, -(float)Assets::get().GetSprite("Tank_Turret")->height / 2.0f); // Translate to be able to rotate on center of sprite
			if (!run)
			{
				player->angleTurret = pi / 2;
			}
			else
			{
				player->angleTurret = atan2(GetMouseY() - player->y, GetMouseX() - player->x) + (pi / 2);
			}
			tTurret.Rotate(player->angleTurret);
			tTurret.Scale(0.1, 0.1);
			tTurret.Translate(player->x, player->y);


			//SetPixelMode(olc::Pixel::MASK);
			//olc::GFX2D::DrawSprite(Assets::get().GetSprite("Tank_Wheels"), tWheels);
			DrawRotatedDecal(olc::vf2d(player->x, player->y), decWheels.get(), player->angleBody, { ((float)Assets::get().GetSprite("Tank_Wheels")->width / 2.0f), (float)Assets::get().GetSprite("Tank_Wheels")->height / 2.0f }, { 0.1f, 0.1f });
			DrawRotatedDecal(olc::vf2d(player->x, player->y), decBody.get(), player->angleBody, { ((float)Assets::get().GetSprite("Tank_Body")->width / 2.0f), (float)Assets::get().GetSprite("Tank_Body")->height / 2.0f }, { 0.1f, 0.1f });
			DrawRotatedDecal(olc::vf2d(player->x, player->y), decTurret.get(), player->angleTurret, { ((float)Assets::get().GetSprite("Tank_Turret")->width / 2.0f), (float)Assets::get().GetSprite("Tank_Turret")->height / 2.0f }, { 0.1f, 0.1f });
			//olc::GFX2D::DrawSprite(Assets::get().GetSprite("Tank_Body"), tBody);
			//olc::GFX2D::DrawSprite(Assets::get().GetSprite("Tank_Turret"), tTurret);
			SetPixelMode(olc::Pixel::NORMAL);
		}
		else // Player is dead
		{
			//Draw fire
			timeFire += fElapsedTime;
			if (timeFire > 0.8f) timeFire = 0.0f;
			if (timeFire >= 0.0f && timeFire < 0.1f)
				DrawRotatedDecal(olc::vf2d(player->x, player->y - 10), decFire1.get(), 0, { sprFire1->width / 2.0f,sprFire1->height / 2.0f }, { 1.0f, 1.0f });
			else if (timeFire > 0.1f && timeFire < 0.2f)
				DrawRotatedDecal(olc::vf2d(player->x, player->y - 10), decFire2.get(), 0, { sprFire1->width / 2.0f,sprFire1->height / 2.0f }, { 1.0f, 1.0f });
			else if (timeFire > 0.2f && timeFire < 0.3f)
				DrawRotatedDecal(olc::vf2d(player->x, player->y - 10), decFire3.get(), 0, { sprFire1->width / 2.0f,sprFire1->height / 2.0f }, { 1.0f, 1.0f });
			else if (timeFire > 0.3f && timeFire < 0.4f)
				DrawRotatedDecal(olc::vf2d(player->x, player->y - 10), decFire4.get(), 0, { sprFire1->width / 2.0f,sprFire1->height / 2.0f }, { 1.0f, 1.0f });
			else if (timeFire > 0.4f && timeFire < 0.5f)
				DrawRotatedDecal(olc::vf2d(player->x, player->y - 10), decFire5.get(), 0, { sprFire1->width / 2.0f,sprFire1->height / 2.0f }, { 1.0f, 1.0f });
			else if (timeFire > 0.5f && timeFire < 0.6f)
				DrawRotatedDecal(olc::vf2d(player->x, player->y - 10), decFire6.get(), 0, { sprFire1->width / 2.0f,sprFire1->height / 2.0f }, { 1.0f, 1.0f });
			else if (timeFire > 0.6f && timeFire < 0.7f)
				DrawRotatedDecal(olc::vf2d(player->x, player->y - 10), decFire7.get(), 0, { sprFire1->width / 2.0f,sprFire1->height / 2.0f }, { 1.0f, 1.0f });
			else
				DrawRotatedDecal(olc::vf2d(player->x, player->y - 10), decFire8.get(), 0, { sprFire1->width / 2.0f,sprFire1->height / 2.0f }, { 1.0f, 1.0f });
		}
		//=========================================================================================================================================================
		
		DrawString(olc::vi2d(ScreenWidth() - 100, 5), "Attempts: " + std::to_string(nLevelAttempts), olc::WHITE); // Show level attempts on screen

		if (!run)
		{
			DrawString(olc::vi2d(5, 5), "PATH PLANNING MODE", olc::DARK_CYAN, 2);
			DrawString(olc::vi2d(10, ScreenHeight() - 55), "Left Click: Place points\nRight Click: Delete Points\nSpace: Run", olc::WHITE, 2);
		}
		else
		{
			timeCursor += fElapsedTime;
			if (timeCursor > 0.8f) timeCursor = 0.0f;
			if (timeCursor >= 0.0f && timeCursor < 0.2f)
				DrawString(olc::vi2d(ScreenWidth() / 3 + 50, 5), "RUNNING", olc::DARK_GREEN, 2);
			else if (timeCursor > 0.2f && timeCursor < 0.4f)
				DrawString(olc::vi2d(ScreenWidth() / 3 + 50, 5), "RUNNING.", olc::DARK_GREEN, 2);
			else if (timeCursor > 0.4f && timeCursor < 0.6f)
				DrawString(olc::vi2d(ScreenWidth() / 3 + 50, 5), "RUNNING..", olc::DARK_GREEN, 2);
			else
				DrawString(olc::vi2d(ScreenWidth() / 3 + 50, 5), "RUNNING...", olc::DARK_GREEN, 2);
		}

		if (player->bDead)
		{
			DrawString(olc::vi2d(ScreenWidth() / 3, ScreenHeight() / 3), "You are dead!\nPress SPACE to try again!", olc::BLUE, 2);
		}
		return true;
	}

	void PathUpdate(float fElapsedTime, Machine* object)
	{
		fTimeSoFar += fElapsedTime;
		float t = fTimeSoFar / fDuration; // Normalized time value
		if (t > 1.0f) t = 1.0f;

		object->x = (object->vPath[pathCnt].x - object->vPath[pathCnt - 1].x) * t + object->vPath[pathCnt - 1].x;
		object->y = (object->vPath[pathCnt].y - object->vPath[pathCnt - 1].y) * t + object->vPath[pathCnt - 1].y;
		object->vx = (object->vPath[pathCnt].x - object->vPath[pathCnt - 1].x) / fDuration; // vx and vy don't actually move the player now, but we're using it for determining sprite angle/direction
		object->vy = (object->vPath[pathCnt].y - object->vPath[pathCnt - 1].y) / fDuration;

		if (fTimeSoFar >= fDuration
			&& object->vPath[pathCnt] != object->vPath.back())
		{
			object->x = object->vPath[pathCnt].x;
			object->y = object->vPath[pathCnt].y;
			object->vx = 0.0f;
			object->vy = 0.0f;

			fTimeSoFar = 0.0f;
			pathCnt++;
		}
		else if (fTimeSoFar >= fDuration
			&& object->vPath[pathCnt] == object->vPath.back())
		{
			object->x = object->vPath.back().x;
			object->y = object->vPath.back().y;
			object->vx = 0.0f;
			object->vy = 0.0f;
			run = false;
			fTimeSoFar = 0.0f;
			pathCnt = 1;
		}
	}

	void ResetLevel()
	{
		run = false;
		fTimeSoFar = 0.0f;
		pathCnt = 1;
		player->x = player->initX;
		player->y = player->initY;

	}

	void ChangeLevel(int n)
	{
		currentLevel = levels[n];
	}

	bool LoadLevelsFromFile(int numLevels)
	{
		for (int i = 0; i < numLevels; i++) // Load every level into RAM
		{
			std::ifstream inFile(std::to_string(i) + ".lvl", std::ios::in | std::ios::binary);
			if (inFile.is_open())
			{
				int caseTest;
				inFile >> caseTest;
				switch (caseTest)
				{
				case 0: // Barrier no damage
					float bX, bY, bWX, bWY;
					inFile >> bX >> bY >> bWX >> bWY;
					levels[i].barriers.push_back(new Barrier(bX, bY, bWX, bWY, false, 0));
					break;
				case 1: // Barrier with Damage
					float bX1, bY1, bWX1, bWY1, bDmg1;
					inFile >> bX1 >> bY1 >> bWX1 >> bWY1 >> bDmg1;
					levels[i].barriers.push_back(new Barrier(bX1, bY1, bWX1, bWY1, true, bDmg1));
					break;
				case 2: // Enemy type 1
					float initX, initY, speed;
					int health;
					break;
				}

				return true;
			}
			else
			{
				std::cout << "Unable to open file." << std::endl;
				inFile.close();
				return false;
			}
		}
	}

	bool BarrierCollisionDetect(Barrier* b)
	{
		if (player->x < b->pos.x + b->size.x && player->x + (sprBody->width * 0.03f) > b->pos.x
			&& player->y < b->pos.y + b->size.y && player->y + (sprBody->height * 2) > b->pos.y)
		{
			run = false;
			player->vx = 0.0f;
			player->vy = 0.0f;
			return true;
		}
		else
			return false;
	}
};


int main()
{
	MvM demo;
	if (demo.Construct(960, 480, 2, 2, false, false, false))
	//if (demo.Construct(480, 240, 4, 4, false, false, false))
		demo.Start();

	return 0;
}

//float fAngle = atan2(-(vPath[pathCnt].y - player->y), vPath[pathCnt].x - player->x);
//player->vx = playerSpeed * cos(fAngle);
//player->vy = playerSpeed * -sin(fAngle);