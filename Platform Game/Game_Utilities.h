#pragma once
#include "olcPixelGameEngine.h"



//Put "utility" functions like the DisplayDialog and others currently in Jerry_Engine


namespace utility
{
	using namespace std;

	// Since the PGEX has bugs as massive performance hit into the level
	olc::Sprite* InvertSprite(olc::Sprite* spr);
}