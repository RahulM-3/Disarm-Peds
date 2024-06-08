#include "script.h"
#include "debug.h"
#include <string>
#include <vector>
#include "hashtostr.h"
#include <sstream>
#include "bone.h"


cachememory cacheset(10);
int bone;

void update()
{
	// player
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed))
		return;

	// get all peds
	const int ARR_SIZE = 1024;
	Ped peds[ARR_SIZE];
	int count = worldGetAllPeds(peds, ARR_SIZE);

	/*for (int i = 0; i < count; i++)
	{
		if (PED::IS_PED_HUMAN(peds[i]) && peds[i] != playerPed)
		{
			int bone;
			if (PED::GET_PED_LAST_DAMAGE_BONE(peds[i], &bone))
			{

			}
		}
	}*/

	// debug
	for (int i = 0; i < count; i++)
	{
		if (PED::IS_PED_HUMAN(peds[i]) && peds[i] != playerPed && PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(player, peds[i]))
		{
			cacheset.add(peds[i]);
		}
	}

	if (!cacheset.isempty())
	{
		std::vector<Ped> debpeds = cacheset.getpeds();
		for (int i = 0; i < debpeds.size(); i++)
		{
			std::string health = std::to_string(ENTITY::GET_ENTITY_HEALTH(debpeds[i]));
			std::string model = dehash(ENTITY::GET_ENTITY_MODEL(debpeds[i]), "ped");

			Vector3 v = ENTITY::GET_ENTITY_COORDS(debpeds[i], TRUE, FALSE);
			Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);

			std::stringstream ss;
			ss.precision(2);
			ss << std::fixed << GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, v.x, v.y, v.z, TRUE);
			std::string dist = ss.str();

			std::string gotshot = "None";
			std::string anyarm = "None";
			if (PED::GET_PED_LAST_DAMAGE_BONE(debpeds[i], &bone))
			{
				//logfile << mp_male__boneNames[bone] << "\n";
				gotshot = mp_male__boneNames[bone];
				anyarm = hand_bone_name[bone];
			}

			std::vector<std::string> text = {health, model, dist, gotshot, anyarm};
			entity_draw_info_add(debpeds[i], text);
		}
	}
	cacheset.update();
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
