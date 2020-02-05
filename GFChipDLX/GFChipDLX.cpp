#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include <algorithm>
#include "dlx/include/dlx/ExactCoverProblem.hpp"
#include "dlx/include/dlx/AlgorithmDLX.hpp"
#include "dlx/include/dlx/LinkedMatrix.hpp"
#include "CJsonObject/CJsonObject.hpp"
#include "chip.h"
#include "util.hpp"
using namespace std;

AlgorithmDLX::Result result;
vector<vector<unsigned>> rows;
int optionalCols;

void solve(int k)
{
	//
	{
		auto problem = ExactCoverProblem::dense(rows);
		AlgorithmDLX dlx(problem);
		auto r = dlx.search();
		result.number_of_solutions += r.number_of_solutions;
		result.solutions.insert(result.solutions.end(), r.solutions.begin(), r.solutions.end());
		result.profile.insert(result.profile.end(), r.profile.begin(), r.profile.end());
		if (k >= optionalCols)
		return;
	}
	for (auto& it : rows[0])
	{
		if (!it)
		{
			it = 1;
			solve(k + 1);
			it = 0;
		}
	}
}

int main(int argc,char** argv)
{
	auto t0 = clock();
    string optFile = "AT4.json", chipFile = "chips.json";
	if(argc >= 3)
	{
		optFile = argv[1];
		chipFile = argv[2];
	}

	// 解析重装配置
	neb::CJsonObject optionObj(readfile(optFile));
	neb::CJsonObject chipsObj(readfile(chipFile));
	neb::CJsonObject tmpObj;
	int width, height;
	Map base;
	vector<int> availableChips;
	optionObj.Get("width", width);
	optionObj.Get("height", height);
	optionObj.Get("chips", tmpObj);
	if(tmpObj.IsArray())
	{
		int t;
		for(auto i = 0;i < tmpObj.GetArraySize();++i)
		{
			tmpObj.Get(i, t);
			availableChips.push_back(t);
		}
	}
	else
	{
		int t;
		optionObj.Get("chips", t);
		availableChips.push_back(t);
	}
	optionObj.Get("map", tmpObj);
	for(auto i = 0;i < tmpObj.GetArraySize();++i)
	{
		base.push_back(tmpObj(i));
	}

	// 解析芯片配置并生成求解矩阵
	if(!chipsObj.IsArray())
	{
		cout << "chips file error !" << endl;
		return 0;
	}
	
	vector<Chip> chips;
	vector<ChipOption> chipOptions;
	chips.emplace_back();
	chipOptions.emplace_back();
	rows.push_back(map2Row(base));
	optionObj.Get("optional", optionalCols);
	for(auto i = 0;i < chipsObj.GetArraySize();++i)
	{
		auto& obj = chipsObj[i];
		Chip chip(obj);
		chip.no = chips.size();
		if (!isIn(chip.chipClass, availableChips))
		{
			continue;
		}
		chips.push_back(chip);
		
		bool ok;
		int direction;
		obj.Get("direction", direction);
		ChipOption copt(0,0,0,chip.no);
		
		for(copt.rotate = 0; copt.rotate < direction;++copt.rotate)
		{
			// 旋转为顺时针
			// x水平向右，y竖直向下，原点左上角
			auto map = chip.rotate90(copt.rotate).map;
			for(copt.x = 0; copt.x < width;++copt.x)
			{
				for(copt.y = 0; copt.y < height;++copt.y)
				{
					auto t = putChip(base, map,	copt.x, copt.y, &ok);
					if(ok)
					{
						chipOptions.push_back(copt);
						rows.push_back(map2Row(t));
					}
				}
			}
		}
	}
	
	// 求解
	solve(0);

	auto t1 = clock();
	cout << "Time:" << (double)(t1 - t0) / CLOCKS_PER_SEC << endl;
	// 打印结果
	auto printSolution = [&](const AlgorithmDLX::Solution& s)
	{
		AlgorithmDLX::Solution res;
		res.resize(rows[0].size(), 0);
		for(auto i = 0;i < s.size();++i)
		{
			auto row = s[i];
			if(!row)
				continue;
			for(auto j = 0;j < rows[row].size();++j)
			{
				if(rows[row][j])
				{
					res[j] = i;
				}
			}
		}
		for(auto i = 0;i < res.size();++i)
		{
			if(i % width == 0)
			{
				cout << endl;
			}
			if(res[i])
			{
				cout << res[i];
			}
			else
			{
				cout << ' ';
			}
		}
		cout << endl;
		for(auto i = 0;i < s.size();++i)
		{
			if(!s[i])
				continue;
			cout << i << ":" << chips[chipOptions[s[i]].no].name << ", ";
		}
		cout << endl;
		system("pause");
	};

	map<vector<unsigned>, vector<unsigned>> slnMap;
	for(const auto& row:result.solutions)
	{
		vector<unsigned> t;
		for (unsigned int i : row)
		{
			if(!i)
				continue;
			t.push_back(chipOptions[i].no);
		}
		sort(t.begin(), t.end());
		slnMap[t] = row;
	}

	// 保存结果
	cout << "Solutions:" << slnMap.size() << endl;
	neb::CJsonObject slnObj("[]");
	for(const auto& it : slnMap)
	{
		//printSolution(it.second);
		neb::CJsonObject sObj("[]");
		for (unsigned int i : it.second)
		{
			if(i == 0)
				continue;
			neb::CJsonObject t;
			const auto& s = chipOptions[i];
			const auto& c = chips[s.no];
			t.Add("ID", c.id);
			t.Add("rotate", s.rotate);
			t.Add("x", s.x);
			t.Add("y", s.y);
			sObj.Add(t);
		}
		slnObj.Add(sObj);
	}
	fstream fout;
	fout.open(optionObj("name") + "-result.json", ios_base::out);
	fout << slnObj.ToString();
}
