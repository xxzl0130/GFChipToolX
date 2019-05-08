#pragma once
#include "solverStdafx.h"
#include <vector>
#include "GFChip.h"

struct SOLVER_EXP PlanElement
{
    int chipType;
    int chipNum;

    PlanElement(int type = GF_CHIP_TYPE_1,int num = 6):
        chipType(type),chipNum(num){}
};
typedef std::vector<PlanElement> Plan;

#define PLAN_2B14_NUM       230
#define PLAN_AGS_NUM        441
#define PLAN_BGM_NUM        219

extern SOLVER_EXP Plan plan2B14[PLAN_2B14_NUM];
extern SOLVER_EXP Plan planAGS[PLAN_AGS_NUM];
extern SOLVER_EXP Plan planBGM[PLAN_BGM_NUM];