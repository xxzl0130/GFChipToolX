#include <iostream>
#include <fstream>
#include <string>
#include "dlx/include/dlx/ExactCoverProblem.hpp"
#include "dlx/include/dlx/AlgorithmDLX.hpp"
#include "dlx/include/dlx/LinkedMatrix.hpp"
#include "CJsonObject/CJsonObject.hpp"
#include "chip.h"
using namespace std;

int main()
{
    fstream fin;
	fin.open("chips.json", ios_base::in);
	string jsonStr, tmp;
	while((bool)(fin >> tmp))
	{
		jsonStr += tmp;
	}
	neb::CJsonObject oJson(jsonStr);
	Chip chip(oJson[1]);
	Map base({ "00000","00000","01101" });
	bool ok;
	for(auto i = 0;i < 3;++i)
	{
		for(auto j = 0;j < 3;++j)
		{
			auto t = putChip(base, chip.map, i, j, &ok);
			if(ok)
			{
				cout << "yes!" << endl;
				for(const auto& it : t)
				{
					cout << it << endl;
				}
			}
			else
			{
				cout << "no" << endl;
			}
		}
	}
}
