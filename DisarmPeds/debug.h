#include <string>
#include <vector>
#include "script.h"

struct text_box_t
{
	std::string text;
	float x, y;
	byte r, g, b, a;
};

void entity_draw_info_add(Entity entity, std::vector<std::string> text)
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
		
		char formatedtext[256];
		formatedtext[0] = '^';
		int i = 1;
		for each (std::string t in text)
		{
			formatedtext[i++] = '\n';
			formatedtext[i++] = '|';
			formatedtext[i++] = ' ';
			for (int j = 0; j < t.size(); j++)
			{
				formatedtext[i++] = t[j];
			}
		}
		//sprintf_s(formatedtext, "^\n| %s %08X\n| Distance %.02f\n| Health %d", type.c_str(), model, dist, health);

		// draw
		UI::SET_TEXT_SCALE(0.2, 0.2);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", const_cast<char*>(formatedtext)), x, y);
		GRAPHICS::DRAW_RECT(x + 0.028f, y + 0.033f, 0.058f, 0.041f, 75, 75, 75, 100, 0, 0);
	}
}