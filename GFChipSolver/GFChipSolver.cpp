#include "GFChipSolver.h"
#include <map>
#include <vector>
#include <algorithm>

//将不同class的芯片分类,key为class，value为编号和是否使用
typedef std::map<int, std::vector<std::pair<int,bool>>> ChipClassifiedMap;
enum WorkType
{
    AllSolutions,
    FiltrateBlocks,
    FiltrateOverflow
};

static WorkType workType;
static ChipClassifiedMap chipClassified;
static std::vector<Solution> solutions;
static Solution tmpSolution;
static const std::vector<GFChip>* chipsPtr = nullptr;
static int planNumber = 0;
static Block targetBlock, maxOver4;
static double maxOver;

//将不同class的芯片分类
void classifyChip(const std::vector<GFChip>& chips);
//检查当前芯片数量是否满足该拼法最低需要
bool satisfyPlan(const Plan& plan);
void findSolution(const Plan& plan, int k = 0);
//统计拼法的数据
void calcSolution(Solution& s);
void add(Solution& s, const GFChip& c);
void sub(Solution& s, const GFChip& c);
//根据WorkType检查当前方案是否满足条件,true为可行
bool checkSolution();
//实际的统一求解入口
void privateSolveChip(const std::vector<GFChip>& chips, const Plans& plans);

std::vector<Solution> solveChip(const std::vector<GFChip>& chips, const Plans& plans)
{
    workType = AllSolutions;
    privateSolveChip(chips, plans);

    return solutions;
}

std::vector<Solution> solveChip(const std::vector<GFChip>& chips, const Plans& plans, const Block& target, double maxOverflow)
{
    targetBlock = target;
    maxOver = maxOverflow;
    workType = FiltrateOverflow;
    privateSolveChip(chips, plans);

    return solutions;
}

std::vector<Solution> solveChip(const std::vector<GFChip>& chips, const Plans& plans, const Block& target,
    const Block& error)
{
    targetBlock = target;
    maxOver4 = error;
    workType = FiltrateBlocks;
    privateSolveChip(chips, plans);

    return solutions;
}

std::vector<int> findSolution(const std::vector<Solution>& solutions, int blockDmg, int blockDbk, int blockAcu, int blockFil)
{
    std::vector<int> indexs;
    for(auto i = 0;i < solutions.size();++i)
    {
        const auto& it = solutions[i];
        if (it.blockDmg == blockDmg && it.blockAcu == blockAcu && it.blockDbk == blockDbk && it.blockFil == blockFil)
        {
            indexs.push_back(i);
        }
    }
    return indexs;
}

void classifyChip(const std::vector<GFChip>& chips)
{
    chipClassified.clear();
    for(auto i = 0;i < chips.size();++i)
    {
        const auto& chip = chips[i];
        if(!chipClassified.count(chip.chipType))
        {
            //创建
            chipClassified[chip.typeId] = std::vector<std::pair<int, bool>>();
        }
        chipClassified[chip.typeId].emplace_back(i,false);
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
        ans = ans && (chipClassified[it.first].size() >= it.second);
    }

    return ans;
}

void findSolution(const Plan& plan, int k)
{
    if(k >= plan.size())
    {
        //calcSolution(tmpSolution);
        tmpSolution.chipNumber = k;
        tmpSolution.planNumber = planNumber;
        solutions.push_back(tmpSolution);
        return;
    }
    auto& chips = chipClassified[plan[k]];//获取当前所需型号的芯片列表
    for(auto& it : chips)
    {
        if (!it.second)
        {
            //芯片未被使用
            it.second = true;
            tmpSolution.chipIndex[k] = it.first;
            add(tmpSolution, (*chipsPtr)[it.first]);
            findSolution(plan, k + 1);
            it.second = false;//恢复记录
            sub(tmpSolution, (*chipsPtr)[it.first]);
        }
    }
}

void calcSolution(Solution& s)
{
    s.blockDbk = 0; s.valueDbk = 0;
    s.blockDmg = 0; s.valueDmg = 0;
    s.blockFil = 0; s.valueFil = 0;
    s.blockAcu = 0; s.valueAcu = 0;
    for(auto i = 0;i < s.chipNumber;++i)
    {
        const auto& chip = (*chipsPtr)[s.chipIndex[i]];
        auto value = chip.calcValue();
        s.blockDbk += chip.blockDbk; 
        s.valueDbk += value.blockDbk;
        s.blockDmg += chip.blockDmg; 
        s.valueDmg += value.blockDmg;
        s.blockFil += chip.blockFil; 
        s.valueFil += value.blockFil;
        s.blockAcu += chip.blockAcu; 
        s.valueAcu += value.blockAcu;
    }
}

void add(Solution& s, const GFChip& c)
{
    s.blockDmg += c.blockDmg;
    s.blockAcu += c.blockAcu;
    s.blockDbk += c.blockDbk;
    s.blockFil += c.blockFil;
    auto v = c.calcValue();
    s.valueDmg += v.blockDmg;
    s.valueAcu += v.blockAcu;
    s.valueDbk += v.blockDbk;
    s.valueFil += v.blockFil;
}

void sub(Solution& s, const GFChip& c)
{
    s.blockDmg -= c.blockDmg;
    s.blockAcu -= c.blockAcu;
    s.blockDbk -= c.blockDbk;
    s.blockFil -= c.blockFil; 
    auto v = c.calcValue();
    s.valueDmg -= v.blockDmg;
    s.valueAcu -= v.blockAcu;
    s.valueDbk -= v.blockDbk;
    s.valueFil -= v.blockFil;
}

bool checkSolution()
{
    switch (workType)
    {
    case AllSolutions:
        return true;
    case FiltrateBlocks:
    {
        bool over = false;
        over = over || (tmpSolution.blockDmg - targetBlock.blockDmg) > maxOver4.blockDmg;
        over = over || (tmpSolution.blockDbk - targetBlock.blockDbk) > maxOver4.blockDbk;
        over = over || (tmpSolution.blockAcu - targetBlock.blockAcu) > maxOver4.blockAcu;
        over = over || (tmpSolution.blockFil - targetBlock.blockFil) > maxOver4.blockFil;
        return !over;
    }
        break;
    case FiltrateOverflow:
    {
        double over = 0;
        over += std::max(0.0, tmpSolution.blockDmg - targetBlock.blockDmg);
        over += std::max(0.0, tmpSolution.blockAcu - targetBlock.blockAcu);
        over += std::max(0.0, tmpSolution.blockDbk - targetBlock.blockDbk);
        over += std::max(0.0, tmpSolution.blockFil - targetBlock.blockFil);
        return over <= maxOver;
    }
    default:
        return false;
    }
}

void privateSolveChip(const std::vector<GFChip>& chips, const Plans& plans)
{
    solutions.clear();
    chipsPtr = &chips;
    classifyChip(chips);
    for (auto i = 0; i < plans.size(); ++i)
    {
        if (!satisfyPlan(plans[i]))
        {
            continue;
        }
        planNumber = i;
        findSolution(plans[i]);
    }
}
