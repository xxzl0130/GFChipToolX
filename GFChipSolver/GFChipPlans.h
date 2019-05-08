#pragma once
#include <vector>
#include "GFChip.h"

struct PlanElement
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

extern Plan plan2B14[PLAN_2B14_NUM];
extern Plan planAGS[PLAN_AGS_NUM];
extern Plan planBGM[PLAN_BGM_NUM];