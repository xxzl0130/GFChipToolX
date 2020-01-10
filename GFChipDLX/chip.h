#pragma once
#include <string>
#include <vector>
#include "CJsonObject/CJsonObject.hpp"

typedef std::vector<std::string> Map;

class Chip
{
public:
	Chip();
	Chip(neb::CJsonObject& oJson);
	~Chip() = default;

	int no;
	int chipClass; //56,551,552,etc
	int width;
	int height;
	int blocks;
	
	std::string name;
	Map map;

	std::vector<unsigned> toRow();
	Chip rotate90(int n = 1) const; // anti-clockwise
};

struct ChipOption
{
	int no;
	int x, y;
	int rotate;
	ChipOption(int _x = 0,int _y = 0,int _r = 0,int _no = 0):no(_no),x(_x),y(_y),rotate(_r){}
};

std::vector<unsigned> map2Row(const Map& map);

Map putChip(const Map& base, const Map& chip, int x, int y, bool* ok = nullptr);