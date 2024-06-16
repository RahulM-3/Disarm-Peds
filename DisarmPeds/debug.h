#include <string>
#include <vector>
#include "script.h"
#include <ctime>
#include <unordered_map>
#include <fstream>
#include <sstream>

std::ofstream logfile("DisarmPeds.log");

std::string precision(float value, int pre=2)
{
	std::stringstream ss;
	ss.precision(pre);
	ss << std::fixed << value;
	return ss.str();
}

float GetPrivateProfileFloatA(char* section, char* key, char* defaultval, char* file)
{
	char rawvalue[256];
	GetPrivateProfileStringA(section, key, defaultval, rawvalue, sizeof(rawvalue), file);
	float value;
	std::stringstream ss(rawvalue);
	ss >> value;
	return value;
}

void onscreen_debug(std::vector<std::string> text, float x, float y)
{
	std::string formatedtext = "On Screen Debug:\n";
	float width = 0.011, height = 0.04;
	float x1 = x + 0.005, y1 = y + 0.02;
	int lines = text.size(), chars = 0;
	for each (auto i in text)
	{
		formatedtext += i + "\n";
		chars = max(chars, i.size());
	}

	width *= chars;
	x1 += 0.005 * chars;
	height *= lines+1;
	y1 += 0.02 * lines;

	UI::SET_TEXT_SCALE(0.6, 0.6);
	UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", const_cast<char*>(formatedtext.c_str())), x, y);

	GRAPHICS::DRAW_RECT(x1, y1, width, height, 75, 75, 75, 150, 0, 0);
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

		float chars = 0, lines = text.size();
		char formatedtext[256];
		int i = 0;
		formatedtext[i++] = '^';
		for each (std::string t in text)
		{
			formatedtext[i++] = '\n';
			formatedtext[i++] = '|';
			formatedtext[i++] = ' ';
			chars = max(t.size(), chars);
			for (int j = 0; j < t.size(); j++)
			{
				formatedtext[i++] = t[j];
			}
		}

		float width = 0.008, height = 0.03;
		float x1 = x + 0.0039, y1 = y + 0.02;
		width *= chars;
		x1 += 0.0036 * chars;
		height *= lines;
		y1 += 0.015 * lines;

		// draw
		UI::SET_TEXT_SCALE(0.4, 0.4);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", const_cast<char*>(formatedtext)), x, y);
		// box
		GRAPHICS::DRAW_RECT(x1, y1, width, height, 75, 75, 75, 110, 0, 0);
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

	float clearcachein(Ped ped)
	{
		return  cachetime - (std::time(0) - cacheset[ped]);
	}

	bool entityoncache(Ped ped)
	{
		return cacheset.find(ped) != cacheset.end();
	}
};