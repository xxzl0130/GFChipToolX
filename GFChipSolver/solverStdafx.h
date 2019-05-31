#pragma once

#ifdef _WINDLL
#define SOLVER_EXP __declspec(dllexport)
#else
#define SOLVER_EXP __declspec(dllimport)
#endif
