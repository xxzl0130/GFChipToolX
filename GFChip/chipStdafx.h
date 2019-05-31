#pragma once
#ifdef _WINDLL
    #define CHIP_EXP __declspec(dllexport)
#else
    #ifdef CHIP_IMPORT
        #define CHIP_EXP __declspec(dllimport)
    #else
        #define CHIP_EXP
    #endif
#endif