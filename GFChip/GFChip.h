#pragma once
#include <string>
#include "chipStdafx.h"

//56
#define GF_CHIP_CLASS_56        56
#define GF_CHIP_TYPE_1          1
#define GF_CHIP_TYPE_2          2
#define GF_CHIP_TYPE_3          3
#define GF_CHIP_TYPE_4A         41
#define GF_CHIP_TYPE_4B         42
#define GF_CHIP_TYPE_5          5
#define GF_CHIP_TYPE_6          6
#define GF_CHIP_TYPE_7          7
#define GF_CHIP_TYPE_8          8
#define GF_CHIP_TYPE_9          9
//551
#define GF_CHIP_CLASS_551       551
#define GF_CHIP_TYPE_FA         11
#define GF_CHIP_TYPE_FB         12
#define GF_CHIP_TYPE_NA         21
#define GF_CHIP_TYPE_NB         22
#define GF_CHIP_TYPE_YA         31
#define GF_CHIP_TYPE_YB         32
#define GF_CHIP_TYPE_T          4
#define GF_CHIP_TYPE_W          5
#define GF_CHIP_TYPE_X          6
//552
#define GF_CHIP_TYPE_B          81
#define GF_CHIP_TYPE_D          82
#define GF_CHIP_TYPE_I          9
#define GF_CHIP_TYPE_C          10
#define GF_CHIP_TYPE_Z          111
#define GF_CHIP_TYPE_Z_         112
#define GF_CHIP_TYPE_V          120
#define GF_CHIP_TYPE_L          131
#define GF_CHIP_TYPE_L_         132

class CHIP_EXP GFChip
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

    //计算属性数值，返回的对象中的block属性不再是格数而是数值。
    GFChip calcValue() const;

    // to excel line by cups
    std::string toExcelLine() const;
    // create from line of cups excel
    void fromExcelLine(const std::string& line);
    // create from line of cups excel
    static GFChip createFromExcelLine(const std::string& line);
private:
};

