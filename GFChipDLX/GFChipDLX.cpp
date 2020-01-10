#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "dlx/include/dlx/ExactCoverProblem.hpp"
#include "dlx/include/dlx/AlgorithmDLX.hpp"
#include "dlx/include/dlx/LinkedMatrix.hpp"
#include "CJsonObject/CJsonObject.hpp"
#include "chip.h"
#include "util.hpp"
using namespace std;

int main(int argc,char** argv)
{
	auto t0 = clock();
    string optFile = "AT4.json", chipFile = "chips.json";
	if(argc >= 3)
	{
		optFile = argv[1];
		chipFile = argv[2];
	}

	neb::CJsonObject optionObj(readfile(optFile));
	neb::CJsonObject chipsObj(readfile(chipFile));
	neb::CJsonObject tmpObj;
	int width, height;
	Map availableChips, base;
	optionObj.Get("width", width);
	optionObj.Get("height", height);
	optionObj.Get("chips", tmpObj);
	if(tmpObj.IsArray())
	{
		for(auto i = 0;i < tmpObj.GetArraySize();++i)
		{
			availableChips.push_back(tmpObj(i));
		}
	}
	else
	{
		availableChips.push_back(optionObj("chips"));
	}
	optionObj.Get("map", tmpObj);
	for(auto i = 0;i < tmpObj.GetArraySize();++i)
	{
		base.push_back(tmpObj(i));
	}

	if(!chipsObj.IsArray())
	{
		cout << "chips file error !" << endl;
		return 0;
	}
	vector<vector<unsigned>> rows;
	int optionalCols;
	vector<Chip> chips;
	vector<ChipOption> chipOptions;
	chips.emplace_back();
	chipOptions.emplace_back();
	rows.push_back(map2Row(base));
	optionObj.Get("optional", optionalCols);
	for(auto i = 0;i < chipsObj.GetArraySize();++i)
	{
		auto& obj = chipsObj[i];
		string cls;
		obj.Get("class", cls);
		if(!isIn(cls,availableChips))
		{
			continue;
		}
		Chip chip(obj);
		chip.no = chips.size();
		chips.push_back(chip);
		
		bool ok;
		auto& rotate = obj["rotate"];
		ChipOption copt(0,0,0,chip.no);
		
		for(auto j = 0;j < rotate.GetArraySize();++j)
		{
			rotate.Get(j, copt.rotate);
			for(copt.x = 0; copt.x < width;++copt.x)
			{
				for(copt.y = 0; copt.y < height;++copt.y)
				{
					auto t = putChip(base, chip.rotate90(copt.rotate).map,
						copt.x, copt.y, &ok);
					if(ok)
					{
						chipOptions.push_back(copt);
						rows.push_back(map2Row(t));
					}
				}
			}
		}
	}

	auto problem = ExactCoverProblem::dense(rows, optionalCols);
	AlgorithmDLX dlx(problem);
	auto result = dlx.search();

	auto t1 = clock();
	cout << "Time:" << (double)(t1 - t0) / CLOCKS_PER_SEC << endl;
	cout << "Solutions:" << result.number_of_solutions << endl;
	for(const auto& row:result.solutions)
	{
		for (const auto& it : row)
		{
			cout << chips[chipOptions[it].no].name << ", ";
		}
		cout << endl;
	}
}
