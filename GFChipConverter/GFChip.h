#pragma once
#include <string>

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

    // to save coed used by GFTool, without end char '&'
    std::string toSaveCode() const;
    // create from save coed used by GFTool, without '&', '[', ']', like "1,1,551,120,0,2,0,1,2,1"
    void fromSaveCode(const std::string& code);
    // create from save coed used by GFTool, without '&', '[', ']', like "1,1,551,120,0,2,0,1,2,1"
    static GFChip createFromSaveCode(const std::string& code);

    // to excel line by cups
    std::string toExcelLine() const;
    // create from line of cups excel
    void fromExcelLine(const std::string& line);
    // create from line of cups excel
    static GFChip createFromExcelLine(const std::string& line);
private:
    static constexpr double argDmg = 4.4, argDbk = 12.7, argAcu = 7.1, argFil = 5.7;
    static constexpr double den56 = 1.0, den551 = 1.0, den552 = 0.92;
    static constexpr double argLv[21] = {1.0,1.08,1.16,1.24,1.32,1.4,1.48,1.56,1.64,1.72,1.8,1.87,1.94,2.01,2.08,2.15,2.22,2.29,2.36,2.43,2.5 };
};

