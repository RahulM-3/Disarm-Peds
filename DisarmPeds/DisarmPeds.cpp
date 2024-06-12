#include "script.h"
#include "debug.h"
#include <string>
#include <vector>
#include "hashtostr.h"

cachememory cacheset(10); // debug cache memory
std::ifstream inifile("DisarmPeds.ini"); // configuration file
// hand bones
std::unordered_map<int, std::string> selective_bones = {
	{46065, "right upper arm"},
	{54187, "right fore arm"},
	{37873, "left upper arm"},
	{53675, "left fore arm"}
};

// main mod function
void disarm(Ped ped)
{
	int bone; // track damaged bone
	Hash weapononright; // get current weapon
	int attach_point = 0;
	PED::GET_PED_LAST_DAMAGE_BONE(ped, &bone);
	WEAPON::GET_CURRENT_PED_WEAPON(ped, &weapononright, NULL, 0, NULL);

	// ped debug
	if (!cacheset.entityoncache(ped))
	{
		return;
	}
	std::string health = std::to_string(ENTITY::GET_ENTITY_HEALTH(ped));
	std::string model = ped_names[ENTITY::GET_ENTITY_MODEL(ped)];

	Vector3 v = ENTITY::GET_ENTITY_COORDS(ped, TRUE, FALSE);
	Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
	std::string dist = "Distance: " + precision(GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, v.x, v.y, v.z, TRUE));
	
	std::string gotshot = "Damaged Bone: " + skelbone_names[bone];
	std::string anyarm = "Hit on any arm: " + selective_bones[bone];
	std::string weapon = "Weapon on right: " + weapon_names[weapononright];
	std::string weapongroup = "Weapon Group: " +weapon_groups[WEAPON::GET_WEAPONTYPE_GROUP(weapononright)];
	std::string timeleft = "Clearing from cache in: " + precision(cacheset.clearcachein(ped)) + " s";

	std::vector<std::string> entitydebug = { health, model, dist, weapon, weapongroup, gotshot, anyarm, timeleft };
	entity_debug(ped, entitydebug);
}

void update()
{
	// player
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;

	// get all peds
	const int ARR_SIZE = 1024;
	Ped peds[ARR_SIZE];
	int count = worldGetAllPeds(peds, ARR_SIZE);


	for (int i = 0; i < count; i++)
	{
		// add peds to cache memory for debug
		if (PED::IS_PED_HUMAN(peds[i]) && peds[i] != playerPed && PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(player, peds[i]))
		{
			cacheset.add(peds[i]);
		}

		disarm(peds[i]);
	}
	
	// debug update
	std::vector<std::string> onscreendebug; // on screen debug values
	// ini file debug
	if (inifile.good())
	{
		onscreendebug.push_back("Ini file: found");
	}
	else
	{
		onscreendebug.push_back("Ini file: not found");
	}
	onscreendebug.push_back("Npc count: " + std::to_string(count));

	cacheset.update();
	onscreendebug.push_back("Npc on cache: " + std::to_string(cacheset.size()));
	onscreen_debug(onscreendebug, 0.75, 0.75);
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