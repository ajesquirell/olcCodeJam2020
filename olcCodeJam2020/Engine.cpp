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
	struct Machine {
		float x;
		float y;
		float vx;
		float vy;
		float angleBody;
		float angleTurret;
	};

	struct Barrier {
		olc::vf2d pos;
		olc::vf2d size;
		Barrier(float x, float y, float wx, float wy)
		{
			pos = olc::vf2d(x, y);
			size = olc::vf2d(wx, wy);
		}
	};

	struct Exit {
		olc::vf2d pos;
		olc::vf2d size;
		Exit(float x, float y, float wx, float wy)
		{
			pos = olc::vf2d(x, y);
			size = olc::vf2d(wx, wy);
		}
	};

	Machine player;
	float initX, initY;
	float playerSpeed;

	std::unique_ptr<olc::Sprite> sprBody;
	std::unique_ptr<olc::Decal> decBody;
	std::unique_ptr<olc::Sprite> sprTurret;
	std::unique_ptr<olc::Decal> decTurret;
	std::unique_ptr<olc::Sprite> sprWheels;
	std::unique_ptr<olc::Decal> decWheels;


	//Barriers
	std::vector<Barrier*> barriers;

	// Exit
	Exit* exit;

	olc::vi2d tileSize = olc::vi2d(1, 1);
	olc::vi2d playerSize = olc::vi2d(16, 16);

	// Path and point Variables
	std::vector<olc::vf2d> vPath;
	bool run = false;
	int pathCnt = 1;
	float fTimeSoFar = 0;
	float fDuration = 3;
	int pointRadius = 2;

	float timeCursor; // For little animation
	float timeCursor2;

	std::string test;

public:
	bool OnUserCreate() override
	{
		initX = 50.0f;
		initY = 60.0f;
		playerSpeed = 20.0f;

		player.x = initX;
		player.y = initY;
		player.vx = 0.0f;
		player.vy = 0.0f;
		player.angleBody = 0.0f;

		vPath.push_back(olc::vf2d(player.x, player.y));

		barriers.push_back(new Barrier(ScreenWidth() - 80, 20, 30, 90));

		exit = new Exit(ScreenWidth() - 30, 0, 30, ScreenHeight());

		

		Assets::get().LoadSprites();

		// Load Player Sprites
		sprBody = std::make_unique<olc::Sprite>("../Sprites/Tank_Body.png");
		sprTurret = std::make_unique<olc::Sprite>("../Sprites/Tank_Turret.png");
		sprWheels = std::make_unique<olc::Sprite>("../Sprites/Tank_Wheels.png");

		// Create Decals
		decBody = std::make_unique<olc::Decal>(sprBody.get());
		decTurret = std::make_unique<olc::Decal>(sprTurret.get());
		decWheels = std::make_unique<olc::Decal>(sprWheels.get());


		//Sound
		//olc::SOUND::InitialiseAudio(44100, 1, 8, 512);
		//olc::SOUND::PlaySample(Assets::get().GetSound("LitLoop"), true); // Plays Sample C loop

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::VERY_DARK_RED);

		//player.vx = 0.0f;
		//player.vy = 0.0f;

		if (IsFocused())
		{
			if (GetKey(olc::Key::UP).bHeld)
			{
				player.vy = -playerSpeed;
			}

			if (GetKey(olc::Key::DOWN).bHeld)
			{
				player.vy = playerSpeed;
			}

			if (GetKey(olc::Key::LEFT).bHeld)
			{
				player.angleBody -= 0.5f * fElapsedTime;
			}

			if (GetKey(olc::Key::RIGHT).bHeld)
			{
				player.angleBody += 0.5f * fElapsedTime;
			}

			if (GetMouse(0).bReleased && !run)
			{
				vPath.push_back(olc::vf2d(GetMouseX(), GetMouseY()));
			}

			if (GetMouse(1).bReleased && !run) // Right click to delete points, but only if not running. Otherwise vector can be out of range if currently going to point we delete
			{
				if (vPath.size() > 1)
					vPath.pop_back();
			}

			if (GetKey(olc::Key::SPACE).bReleased)
			{
				if (!run && vPath.size() > 1) // Only run if not already running
				{
					run = true;
					//player.x = initX;
					//player.y = initY;
				}
			}
		}

		// Run path when ready
		if (run)
			PathUpdate(fElapsedTime);

		player.x += player.vx * fElapsedTime;
		player.y += player.vy * fElapsedTime;

		// Draw Barriers
		for (int i = 0; i < barriers.size(); i++)
		{
			FillRect(barriers[i]->pos, barriers[i]->size, olc::VERY_DARK_GREY);
			FillRect(barriers[i]->pos + olc::vf2d(2, 2), barriers[i]->size - olc::vf2d(4, 4), olc::GREY);
		}

		// Draw Exit
		FillRect(exit->pos, exit->size, olc::DARK_CYAN);
		olc::Sprite* sprExit = new olc::Sprite(35, 8);
		SetDrawTarget(sprExit);
		Clear(olc::DARK_CYAN);
		DrawString({0, 0}, "EXIT", olc::VERY_DARK_CYAN);
		SetDrawTarget(nullptr);
		olc::Decal* decExit = new olc::Decal(sprExit);
		SetPixelMode(olc::Pixel::MASK);
		DrawRotatedDecal(exit->pos + olc::vf2d(exit->size.x / 2, exit->size.y / 2 - 5), decExit, (pi / 2), { sprExit->width / 2.0f, sprExit->height / 2.0f }, { 1.3f, 1.3f });
		SetPixelMode(olc::Pixel::NORMAL);

		// Draw Path Rays
		for (int i = 0; i < vPath.size() - 1; i++)
		{
			DrawLine(vPath[i], vPath[i + 1], olc::GREEN);
		}

		// Draw Path Points
		for (int i = 1; i < vPath.size(); i++)
		{
			FillCircle(vPath[i], pointRadius, olc::YELLOW);
		}

		if (!run) // When in planning mode, show possible point placement under mouse
		{
			// Line animation
			timeCursor2 += fElapsedTime;
			if (timeCursor2 > 0.6f) timeCursor2 = 0.0f;
			float t = timeCursor2 / 0.6f;
			
			//DrawLine(vPath.back(), olc::vi2d((GetMouseX() - vPath.back().x) * t + vPath.back().x, (GetMouseY() - vPath.back().y) * t + vPath.back().y), olc::WHITE, 0b10001000100010001000100010001000U);
			if (timeCursor2 >= 0.0f && timeCursor2 < 0.15f)
				DrawLine(vPath.back(), olc::vi2d(GetMouseX(), GetMouseY()), olc::MAGENTA, 0b10001000100010001000100010001000U);
			else if (timeCursor2 > 0.15f && timeCursor2 < 0.3f)
				DrawLine(vPath.back(), olc::vi2d(GetMouseX(), GetMouseY()), olc::MAGENTA, 0b01000100010001000100010001000100U);
			else if (timeCursor2 > 0.3f && timeCursor2 < 0.45f)
				DrawLine(vPath.back(), olc::vi2d(GetMouseX(), GetMouseY()), olc::MAGENTA, 0b00100010001000100010001000100010U);
			else
				DrawLine(vPath.back(), olc::vi2d(GetMouseX(), GetMouseY()), olc::MAGENTA, 0b00010001000100010001000100010001U);

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
		//FillRect(olc::vf2d(player.x - (playerSize.x / 2), player.y - (playerSize.y / 2)), playerSize, olc::RED);
		olc::GFX2D::Transform2D tBody;
		olc::GFX2D::Transform2D tTurret;
		olc::GFX2D::Transform2D tWheels;

		// BODY and WHEELS
		tBody.Translate(-(float)Assets::get().GetSprite("Tank_Body")->width / 2.0f, -(float)Assets::get().GetSprite("Tank_Body")->height / 2.0f); // Translate to be able to rotate on center of sprite
		tWheels.Translate(-(float)Assets::get().GetSprite("Tank_Wheels")->width / 2.0f, -(float)Assets::get().GetSprite("Tank_Wheels")->height / 2.0f); // Translate to be able to rotate on center of sprite

		if (!run)
		{
			player.angleBody = pi / 2;
		}
		else
		{
			//player.angleBody = atan2(-(vPath[pathCnt].y - player.y), vPath[pathCnt].x - player.x);
			//player.angleBody = atan2(-player.vy, player.vx) - (pi/2);
			player.angleBody = atan2(player.vy, player.vx) + (pi/2);
		}
		tBody.Rotate(player.angleBody);
		tBody.Scale(0.1, 0.1);
		tBody.Translate(player.x, player.y);

		tWheels.Rotate(player.angleBody);
		tWheels.Scale(0.1, 0.1);
		tWheels.Translate(player.x, player.y);

		// TURRET
		tTurret.Translate(-(float)Assets::get().GetSprite("Tank_Turret")->width / 2.0f, -(float)Assets::get().GetSprite("Tank_Turret")->height / 2.0f); // Translate to be able to rotate on center of sprite
		if (!run)
		{
			player.angleTurret = pi / 2;
		}
		else
		{
			player.angleTurret = atan2(GetMouseY() - player.y, GetMouseX() - player.x) + (pi / 2);
		}
		tTurret.Rotate(player.angleTurret);
		tTurret.Scale(0.1, 0.1);
		tTurret.Translate(player.x, player.y);


		SetPixelMode(olc::Pixel::MASK);
		//olc::GFX2D::DrawSprite(Assets::get().GetSprite("Tank_Wheels"), tWheels);
		DrawRotatedDecal(olc::vf2d(player.x, player.y), decWheels.get(), player.angleBody, { ((float)Assets::get().GetSprite("Tank_Wheels")->width / 2.0f), (float)Assets::get().GetSprite("Tank_Wheels")->height / 2.0f }, { 0.1f, 0.1f });
		DrawRotatedDecal(olc::vf2d(player.x, player.y), decBody.get(), player.angleBody, { ((float)Assets::get().GetSprite("Tank_Body")->width / 2.0f), (float)Assets::get().GetSprite("Tank_Body")->height / 2.0f }, { 0.1f, 0.1f });
		DrawRotatedDecal(olc::vf2d(player.x, player.y), decTurret.get(), player.angleTurret, { ((float)Assets::get().GetSprite("Tank_Turret")->width / 2.0f), (float)Assets::get().GetSprite("Tank_Turret")->height / 2.0f }, { 0.1f, 0.1f });
		//olc::GFX2D::DrawSprite(Assets::get().GetSprite("Tank_Body"), tBody);
		//olc::GFX2D::DrawSprite(Assets::get().GetSprite("Tank_Turret"), tTurret);
		SetPixelMode(olc::Pixel::NORMAL);
		//=========================================================================================================================================================

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
		return true;
	}

	void PathUpdate(float fElapsedTime)
	{
		fTimeSoFar += fElapsedTime;
		float t = fTimeSoFar / fDuration; // Normalized time value
		if (t > 1.0f) t = 1.0f;

		player.x = (vPath[pathCnt].x - vPath[pathCnt - 1].x) * t + vPath[pathCnt - 1].x;
		player.y = (vPath[pathCnt].y - vPath[pathCnt - 1].y) * t + vPath[pathCnt - 1].y;
		player.vx = (vPath[pathCnt].x - vPath[pathCnt - 1].x) / fDuration; // vx and vy don't actually move the player now, but we're using it for determining sprite angle/direction
		player.vy = (vPath[pathCnt].y - vPath[pathCnt - 1].y) / fDuration;

		if (fTimeSoFar >= fDuration
			&& vPath[pathCnt] != vPath.back())
		{
			player.x = vPath[pathCnt].x;
			player.y = vPath[pathCnt].y;
			player.vx = 0.0f;
			player.vy = 0.0f;

			fTimeSoFar = 0.0f;
			pathCnt++;
		}
		else if (fTimeSoFar >= fDuration
			&& vPath[pathCnt] == vPath.back())
		{
			player.x = vPath.back().x;
			player.y = vPath.back().y;
			player.vx = 0.0f;
			player.vy = 0.0f;
			run = false;
			fTimeSoFar = 0.0f;
			pathCnt = 1;
		}
	}
};


int main()
{
	MvM demo;
	if (demo.Construct(960, 480, 2, 2, true, false, false))
	//if (demo.Construct(480, 240, 4, 4, false, false, false))
		demo.Start();

	return 0;
}

//float fAngle = atan2(-(vPath[pathCnt].y - player.y), vPath[pathCnt].x - player.x);
//player.vx = playerSpeed * cos(fAngle);
//player.vy = playerSpeed * -sin(fAngle);