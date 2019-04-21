#pragma once
#include <string>

// chip save code rules by GFTool
extern const std::string rules[2];

class GFChip
{
public:
    GFChip();
    GFChip(int chipNum, int chipColor, int chipClass, int chipType, int chipLevel, int blockAcu, int blockFil,
           int blockDmg, int blockDbk, int weight);
    virtual ~GFChip();

    // keep definitions same with GFTool
    int chipNum;        // chip number
    int chipColor;      // color
    int chipClass;      // 56 for 5-star-6-block, 551 for 5-star-5-block-1-type
    int chipType;       // shape
    int chipLevel;      // strengthen level
    int blockAcu;       // accuracy block num
    int blockFil;       // filling block num
    int blockDmg;       // damage block num
    int blockDbk;       // defence-breaking block num
    int weight;         // weight, currently not used, keep same with GFTool

    // to save coed used by GFTool
    std::string toSaveCode();
    // create from save coed used by GFTool
    void fromSaveCode(const std::string& code);
    // create from save coed used by GFTool
    static GFChip createFromSaveCode(const std::string& code);

    // to excel line by cups
    std::string toExcelLine();
    // create from line of cups excel
    void fromExcelLine(const std::string& line);
    // create from line of cups excel
    static GFChip createFromExcelLine(const std::string& line);
};

