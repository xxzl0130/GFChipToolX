#pragma once
#include "solverStdafx.h"
#include <vector>
#include "GFChip.h"

typedef std::vector<int> Plan;
typedef std::vector<Plan> Plans;

#define PLAN_2B14_NUM       230
#define PLAN_AGS_NUM        441
#define PLAN_BGM_NUM        219

extern SOLVER_EXP Plans plan2B14;
extern SOLVER_EXP Plans planAGS;
extern SOLVER_EXP Plans planBGM;