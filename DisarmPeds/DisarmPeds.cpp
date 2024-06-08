#include "script.h"
#include "debug.h"
#include <string>
#include <vector>
#include "hashtostr.h"

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
		{
			std::string health = std::to_string(ENTITY::GET_ENTITY_HEALTH(peds[i]));
			std::string model = dehash(ENTITY::GET_ENTITY_MODEL(peds[i]), "ped");

			Vector3 v = ENTITY::GET_ENTITY_COORDS(peds[i], TRUE, FALSE);
			Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
			std::string dist = std::to_string(GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, v.x, v.y, v.z, TRUE));
			
			std::vector<std::string> text = {health, model, dist};
			entity_draw_info_add(peds[i], text);
		}
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
