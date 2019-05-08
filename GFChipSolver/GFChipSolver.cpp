#include "GFChipSolver.h"
#include <map>
#include <vector>
#include <algorithm>

//将不同class的芯片分类,key为class，value为编号和是否使用
typedef std::map<int, std::vector<std::pair<int,bool>>> ChipClassifiedMap;
static ChipClassifiedMap chipClassified;
static std::vector<Solution> solutions;
static Solution tmpSolution;

//将不同class的芯片分类
void classifyChip(const std::vector<GFChip>& chips);
//检查当前芯片数量是否满足该拼法最低需要
bool satisfyPlan(const Plan& plan);
void findSolution(const Plan& plan, int k = 0);
//统计拼法的数据
void calcSolution(Solution& s);

std::vector<Solution> solveChip(const std::vector<GFChip>& chips, const Plans& plans)
{
    solutions.clear();
    classifyChip(chips);
    for(auto i = 0;i < plans.size();++i)
    {
        if(!satisfyPlan(plans[i]))
        {
            continue;
        }
        findSolution(plans[i]);
    }

    return solutions;
}

void classifyChip(const std::vector<GFChip>& chips)
{
    chipClassified.clear();
    for(auto i = 0;i < chips.size();++i)
    {
        const auto& chip = chips[i];
        if(!chipClassified.count(chip.chipClass))
        {
            //创建
            chipClassified[chip.chipClass] = std::vector<std::pair<int, bool>>();
        }
        chipClassified[chip.chipClass].emplace_back(i,false);
    }
}

bool satisfyPlan(const Plan& plan)
{
    bool ans = true;
    std::map<int, int> require;
    for(const auto& it : plan)
    {
        ++require[it];
    }

    for(const auto& it : require)
    {
        //每种芯片需求都满足
        ans = ans && (chipClassified.count(it.first) >= it.second);
    }

    return ans;
}

void findSolution(const Plan& plan, int k)
{
    if(k >= plan.size())
    {
        calcSolution(tmpSolution);
        solutions.push_back(tmpSolution);
        return;
    }
    auto& chips = chipClassified[plan[k]];//获取当前所需型号的芯片列表
    for(auto& it : chips)
    {
        if(!it.second)
        {
            //芯片未被使用
            it.second = true;
            tmpSolution.chipIndex[k] = it.first;
            findSolution(plan, k + 1);
            it.second = false;//恢复记录
        }
    }
}

void calcSolution(Solution& s)
{
}
