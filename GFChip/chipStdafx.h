#pragma once
#ifdef _LIB
#define CHIP_EXP __declspec(dllexport)
#else
#define CHIP_EXP __declspec(dllimport)
#endif