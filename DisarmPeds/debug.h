#include <string>
#include <vector>
#include "script.h"

struct text_box_t
{
	std::string text;
	float x, y;
	byte r, g, b, a;
};

void entity_draw_info_add(std::vector<text_box_t>& textOnScreen, Entity entity, float mindist, float maxdist, std::string type, byte r, byte g, byte b, byte a)
{
	Vector3 v = ENTITY::GET_ENTITY_COORDS(entity, TRUE, FALSE);
	float x, y;
	if (GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(v.x, v.y, v.z, &x, &y))
	{
		// draw bounds
		if (x < 0.01 || y < 0.01 || x > 0.93 || y > 0.93)
			return;
		// get coords
		Vector3 plv = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE, FALSE);
		float dist = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(plv.x, plv.y, plv.z, v.x, v.y, v.z, TRUE);
		// draw text if entity isn't close to the player
		if (dist > mindist && dist < maxdist)
		{
			// check if the text fits on screen
			bool bFitsOnscreen = true;
			for each (auto & iter in textOnScreen)
			{
				float textDist = sqrtf((iter.x - x) * (iter.x - x) + (iter.y - y) * (iter.y - y));
				if (textDist < 0.05)
				{
					bFitsOnscreen = false;
					break;
				}
			}
			// if text doesn't fit then skip draw
			if (!bFitsOnscreen) return;
			// add info to the vector
			int health = ENTITY::GET_ENTITY_HEALTH(entity);
			Hash model = ENTITY::GET_ENTITY_MODEL(entity);
			char text[256];
			sprintf_s(text, "^\n| %s %08X\n| Distance %.02f\n| Health %d", type.c_str(), model, dist, health);
			textOnScreen.push_back({ text, x, y, r, g, b, a });
		}
	}
}