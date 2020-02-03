#include "chip.h"
using namespace std;

Chip::Chip(): no(0), chipClass(5061), width(0), height(0), blocks(0), id(0)
{
}

Chip::Chip(neb::CJsonObject& oJson)
{
	this->no = 0;
	oJson.Get("class", this->chipClass);
	oJson.Get("width", this->width);
	oJson.Get("height", this->height);
	oJson.Get("blocks", this->blocks);
	oJson.Get("name", this->name);
	oJson.Get("ID", this->id);
	auto oMap = oJson["map"];
	for(auto i = 0;i < oMap.GetArraySize();++i)
	{
		this->map.push_back(oMap(i));
	}
}

vector<unsigned> Chip::toRow()
{
	return map2Row(map);
}

Chip Chip::rotate90(int n) const
{
	auto rotate = [](const Map& map) -> vector<string>
	{
		Map r;
		string t;
		t.resize(map.size(), '0');
		r.resize(map[0].length(), t);
		for (auto i = 0u; i < r.size(); ++i)
		{
			for (auto j = 0u; j < r[i].length(); ++j)
			{
				r[i][j] = map[map.size() - j - 1][i];
			}
		}
		return r;
	};
	Chip t = *this;
	for(auto i = 0;i < n;++i)
	{
		t.map = rotate(t.map);
	}
	return t;
}

std::vector<unsigned> map2Row(const Map& map)
{
	vector<unsigned> row;
	for (const auto& r : map)
	{
		for (const auto& it : r)
		{
			row.push_back(it - '0');
		}
	}
	return row;
}

Map putChip(const Map& base, const Map& chip, int x, int y, bool* ok)
{
	Map ret;
	string t;
	if(x + chip[0].length() > base[0].length() || y + chip.size() > base.size())
	{
		if (ok != nullptr)
			*ok = false;
		return ret;
	}
	t.resize(base[0].length(), '0');
	ret.resize(base.size(), t);
	for(auto i = 0u;i < chip.size();++i)
	{
		for(auto j = 0u;j < chip[i].length();++j)
		{
			auto c = chip[i][j];
			if(c == '1' && base[i + y][j + x] == '1')
			{
				if(ok != nullptr)
				{
					*ok = false;
				}
				return ret;
			}
			ret[i + y][j + x] = c;
		}
	}
	if (ok != nullptr)
	{
		*ok = true;
	}
	return ret;
}
