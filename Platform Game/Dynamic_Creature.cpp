#include "Dynamics.h"
#include "Assets.h"

cDynamic_Creature::cDynamic_Creature(std::string n) : cDynamic(n)
{
	nHealth = 10;
	nHealthMax = 10;
	fFaceDir = +1.0f;
	bSquat = false;

	animations.mapStates["idle"].push_back(Assets::get().GetSprite("Jerry_Idle"));

	animations.mapStates["run"].push_back(Assets::get().GetSprite("Jerry_Run_1"));
	animations.mapStates["run"].push_back(Assets::get().GetSprite("Jerry_Run_2"));
	animations.mapStates["run"].push_back(Assets::get().GetSprite("Jerry_Run_3"));
	animations.mapStates["run"].push_back(Assets::get().GetSprite("Jerry_Run_4"));

	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_1"));
	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_2"));
	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_3"));
	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_4"));
	animations.mapStates["brake"].push_back(Assets::get().GetSprite("Jerry_Brake_5"));

	animations.mapStates["squat"].push_back(Assets::get().GetSprite("Jerry_Squat"));

	animations.mapStates["jump"].push_back(Assets::get().GetSprite("Jerry_Jump_1"));
	animations.mapStates["jump"].push_back(Assets::get().GetSprite("Jerry_Jump_2"));


	animations.mapStates["fall"].push_back(Assets::get().GetSprite("Jerry_Fall"));

	animations.ChangeState("idle");
}

void cDynamic_Creature::DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) //Screen space offset, since we already have position as data member
{
	olc::GFX2D::Transform2D t;
	t.Translate(-22 / 2, -22 / 2); //Align sprite to 0,0 to do affine transformations
	t.Scale(fFaceDir, 1.0f); //BUG WITH THIS??? CUTS OFF A RIGHT COLUMN OF PIXELS WHEN REFLECTED? Yeah bug is in the PGEX/Scaling transformation somewhere. Could just double the png's used and switch like that instead of scaling

	//t.Rotate(fPlayerPosX);
	t.Translate((px - ox) * 22 + 22/2, (py - oy) * 22 + 22/2); //If we want to change resolution later, need to make a global constant instead of 22

	animations.DrawSelf(pge, t);
}

void cDynamic_Creature::Update(float fElapsedTime, cDynamic* player)
{
	if (bObjectOnGround)
	{
		if (fabs(vx) == 0.0f)
		{
			animations.ChangeState("idle");

			if (fabs(vx) > 0.01f) //LITERALLY all this does is allow him to "dance" by trying to move when up against a wall, probably shouldn't have this here but I think it's funny
				animations.ChangeState("run");
		}
		else if (fFaceDir == +1.0f && vx < 0 || fFaceDir == -1.0f && vx > 0) //Just changed direction but still moving the opposite way -> braking
		{
			animations.ChangeState("brake");
		}
		else
		{
			animations.ChangeState("run");
		}

		if (bSquat) //This is changed in the Input Handling section
			animations.ChangeState("squat");
	}
	else
	{
		if (vy < 0)
		{
			animations.ChangeState("jump", true);
		}
		else
		{
			animations.ChangeState("fall");
		}
	}

	// Update facing direction (Except for player - this is handled explicitly in the game engine using user input)
	if (sName != "Jerry")
		fFaceDir = (vx < 0 ? -1.0f : vx > 0 ? 1.0f : fFaceDir);


	/*if (nHealth <= 0)
		animations.ChangeState("dead");*/


	//Change animation speed based on object's vx -- Could put this in just the player class if we don't want all objects doing this
	if (animations.sCurrentState == "run")
		animations.fTimeBetweenFrames = 0.1f * (10.0f / fabs(vx));
	else if (animations.sCurrentState == "brake")
		animations.fTimeBetweenFrames = 0.05f;
	else
		animations.fTimeBetweenFrames = 0.1f;

	animations.Update(fElapsedTime); //Update animation frame

	Behavior(fElapsedTime, player);
}

void cDynamic_Creature::Behavior(float fElapsedTime, cDynamic* player)
{
	//No default behavior
}