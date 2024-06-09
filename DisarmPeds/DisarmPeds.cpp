#include "script.h"
#include "debug.h"
#include <string>
#include <vector>
#include "hashtostr.h"
#include <sstream>
#include "bone.h"

cachememory cacheset(10);
int bone;
std::ifstream inifile("DisarmPeds.ini");

void update()
{
	// player
	//int right_upper = GetPrivateProfileIntA("Settings", "right_upper", 99, ".\\DisarmPeds.ini");
	//onscreen_debug(std::to_string(right_upper), 0.50, 0.50);
	
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;

	// get all peds
	const int ARR_SIZE = 1024;
	Ped peds[ARR_SIZE];
	int count = worldGetAllPeds(peds, ARR_SIZE);

	std::vector<std::string> onscreendebug; //on screen debug values
	if (inifile.good())
	{
		onscreendebug.push_back("Ini file: found");
	}
	else
	{
		onscreendebug.push_back("Ini file: not found");
	}
	onscreendebug.push_back("Npc count: " + std::to_string(count));


	// main
	
	if (PED::GET_PED_LAST_DAMAGE_BONE(playerPed, &bone))
	{
		onscreendebug.push_back("Bone Damage: " + mp_male__boneNames[bone]);
	}
	else 
	{
		onscreendebug.push_back("Bone Damage: None");
	}


	// debug
	/*for (int i = 0; i < count; i++)
	{
		if (PED::IS_PED_HUMAN(peds[i]) && peds[i] != playerPed && PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(player, peds[i]))
		{
			cacheset.add(peds[i]);
		}
	}
	onscreendebug.push_back("Npc on cache: " + std::to_string(cacheset.size()));
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
				if (anyarm == "")
				{
					anyarm = "None";
				}
			}

			std::vector<std::string> text = {health, model, dist, gotshot, anyarm};
			entity_debug(debpeds[i], text);
		}
	}
	cacheset.update();*/

	onscreen_debug(onscreendebug, 0.60, 0.60);
}

void main()
{	
	//std::ifstream inifile("DisarmPeds.ini");
	//logfile << inifile.good() << "\n";

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
