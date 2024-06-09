#include <string>
#include <vector>
#include "script.h"
#include <ctime>
#include <unordered_map>
#include <fstream>
#include <sstream>

float GetPrivateProfileFloatA()
{

}

void onscreen_debug(std::vector<std::string> text, float x, float y)
{
	int textheight = (text.size() + 1), textwidth = 0;
	char rawheight[256];
	char rawwidth[256];
	char rawx[256];
	char rawy[256];
	GetPrivateProfileStringA("test", "height", "", rawheight, sizeof(rawheight), ".\\DisarmPeds.ini");
	GetPrivateProfileStringA("test", "width", "", rawwidth, sizeof(rawwidth), ".\\DisarmPeds.ini");
	GetPrivateProfileStringA("test", "x", "", rawx, sizeof(rawx), ".\\DisarmPeds.ini");
	GetPrivateProfileStringA("test", "y", "", rawy, sizeof(rawy), ".\\DisarmPeds.ini");
	
	float heightmul, widthmul, x1, y1;
	std::stringstream ssh(rawheight);
	ssh >> heightmul;
	std::stringstream ssw(rawwidth);
	ssw >> widthmul;
	std::stringstream ssx(rawx);
	ssx >> x1;
	std::stringstream ssy(rawy);
	ssy >> y1;

	std::string formatedtext = "On Screen Debug:\n";
	for each (auto i in text)
	{
		formatedtext += i + "\n";
		widthmul = max(widthmul, i.size());
	}

	textwidth /= widthmul;
	textheight /= heightmul;

	formatedtext += "Height: " + std::to_string(textheight) + "\nWidth: " + std::to_string(textwidth);
	UI::SET_TEXT_SCALE(0.6, 0.6);
	UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", const_cast<char*>(formatedtext.c_str())), x, y);

	GRAPHICS::DRAW_RECT(x1, y1, textwidth, textheight, 75, 75, 75, 300, 0, 0);
}

void entity_debug(Entity entity, std::vector<std::string> text)
{
	Vector3 v = ENTITY::GET_ENTITY_COORDS(entity, TRUE, FALSE);
	float x, y;
	if (GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(v.x, v.y, v.z, &x, &y))
	{
		// draw bounds
		if (x < 0.01 || y < 0.01 || x > 0.93 || y > 0.93)
			return;

		float widthmul = 0, heightmul = text.size();
		char formatedtext[256];
		formatedtext[0] = '^';
		int i = 1;
		for each (std::string t in text)
		{
			formatedtext[i++] = '\n';
			formatedtext[i++] = '|';
			formatedtext[i++] = ' ';
			widthmul = max(t.size(), widthmul);
			for (int j = 0; j < t.size(); j++)
			{
				formatedtext[i++] = t[j];
			}
		}

		// draw
		UI::SET_TEXT_SCALE(0.4, 0.4);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", const_cast<char*>(formatedtext)), x, y);
	}
}

class cachememory
{
private:
	std::unordered_map<Ped, time_t> cacheset;
	float cachetime;

public:
	cachememory(float time)
	{
		cachetime = time;
	}

	void add(Ped ped)
	{
		cacheset[ped] = std::time(0);
	}

	int update()
	{
		int removed = 0;
		for (auto i = cacheset.begin(); i != cacheset.end(); i++)
		{
			if (std::time(0) - i->second >= cachetime)
			{
				removed++;
				cacheset.erase(i);
			}
		}
		return removed;
	}

	int size()
	{
		return cacheset.size();
	}

	bool isempty()
	{
		if (cacheset.size() == 0)
		{
			return true;
		}
		return false;
	}

	std::vector<time_t> gettime()
	{
		std::vector<time_t> time;
		for (auto i = cacheset.begin(); i != cacheset.end(); i++)
		{
			time.push_back(i->second);
		}
		return time;
	}

	std::vector<Ped> getpeds()
	{
		std::vector<Ped> peds;
		for (auto i = cacheset.begin(); i != cacheset.end(); i++)
		{
			peds.push_back(i->first);
		}
		return peds;
	}
};