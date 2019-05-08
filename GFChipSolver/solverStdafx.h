#pragma once

#ifdef _USRDLL
#define SOLVER_EXP __declspec(dllexport)
#else
#define SOLVER_EXP __declspec(dllimport)
#endif
