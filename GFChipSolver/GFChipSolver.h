#pragma once
#include "solverStdafx.h"
#include <vector>
#include "GFChip.h"
#include "GFChipPlans.h"

//每个Solution第0个为总属性，后续为芯片
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

SOLVER_EXP std::vector<Solution> solveChip(const std::vector<GFChip>& chips,const Plans& plans);