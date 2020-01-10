#pragma once
#include <vector>
#include <string>
#include <fstream>

template <typename T>
bool isIn(const T& x, const std::vector<T> list)
{
	for (const auto& it : list)
	{
		if (it == x)
			return true;
	}
	return false;
}

std::string readfile(const std::string& filename)
{
	std::fstream fin;
	fin.open(filename, std::ios_base::in);
	if (!fin.is_open())
		return "";
	std::string str, t;
	while(std::getline(fin,t))
	{
		str += t;
	}
	return str;
}