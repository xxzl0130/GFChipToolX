#pragma once
#include "solverStdafx.h"
#include <vector>
#include "GFChip.h"
#include "GFChipPlans.h"

struct SOLVER_EXP Solution
{
    //拼法使用的芯片序号（vector中的序号，不是芯片编号）
    int chipIndex[8];
    int chipNumber;     // 使用的芯片总数
    int blockAcu;       // 精度格数
    int blockFil;       // 装填格数
    int blockDmg;       // 伤害格数
    int blockDbk;       // 破防格数
    int valueAcu;       // 精度数值
    int valueFil;       // 装填数值
    int valueDmg;       // 伤害数值
    int valueDbk;       // 破防数值
    int planNumber;     // 解法编号

    Solution()
    {
        memset(chipIndex, -1, sizeof chipIndex);
        chipNumber = 0;
        blockAcu = 0;
        blockFil = 0;
        blockDmg = 0;
        blockDbk = 0;
        valueAcu = 0;
        valueFil = 0;
        valueDmg = 0;
        valueDbk = 0;
        planNumber = 0;
    }
};

//属性格数
struct SOLVER_EXP Block
{
    double blockDmg, blockDbk, blockAcu, blockFil;
    Block(double dmg = 0.0,double dbk = 0.0,double acu = 0.0,double fil = 0.0):
        blockDmg(dmg), blockDbk(dbk), blockAcu(acu), blockFil(fil){}
};

//按照预设的plans寻找所有解
SOLVER_EXP std::vector<Solution> solveChip(const std::vector<GFChip>& chips,const Plans& plans);
//寻找与target的溢出不超过maxOverflow的所有解
SOLVER_EXP std::vector<Solution> solveChip(const std::vector<GFChip>& chips,const Plans& plans, const Block& target, double maxOverflow = 2.0);
//寻找与target的4项溢出不超过error的所有解
SOLVER_EXP std::vector<Solution> solveChip(const std::vector<GFChip>& chips,const Plans& plans, const Block& target, const Block& error);
//按格数筛选，返回下标
SOLVER_EXP std::vector<int> findSolution(const std::vector<Solution>& solutions, int blockDmg, int blockDbk, int blockAcu, int blockFil);