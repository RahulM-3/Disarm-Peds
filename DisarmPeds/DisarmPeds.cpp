#include "script.h"
#include "debug.h"

void update()
{
	// player
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed))
		return;

	// there are lots of entities in some places so we need to
	// remove possibilty of text being drawn on top of another text
	// thats why we will check distance between text on screen
	std::vector<text_box_t> textOnScreen;

	// get all peds
	const int ARR_SIZE = 1024;
	Ped peds[ARR_SIZE];
	int count = worldGetAllPeds(peds, ARR_SIZE);

	// let's track all animals
	for (int i = 0; i < count; i++)
	{
		if (PED::IS_PED_HUMAN(peds[i]))
			entity_draw_info_add(textOnScreen, peds[i], 15, 9999, "A", 75, 110, 75, 75);
	}
	// draw
	for each (auto &iter in textOnScreen)
	{
		UI::SET_TEXT_SCALE(0.2, 0.2);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", const_cast<char *>(iter.text.c_str())), iter.x, iter.y);
		// box
		GRAPHICS::DRAW_RECT(iter.x + 0.028f, iter.y + 0.033f, 0.058f, 0.041f, iter.r, iter.g, iter.b, iter.a, 0, 0);
	}
}

void main()
{		
	while (true)
	{
		update();
		WAIT(0);
	}
}

void ScriptMain()
{	
	srand(GetTickCount64());
	main();
}
