#include "GFChip.h"
using namespace std;

const string rules[2] = { "InfinityFrost", "FatalChapters" };

GFChip::GFChip():chipNum(0), chipColor(0), chipClass(0), chipType(0), chipLevel(0), blockAcu(0),
blockFil(0), blockDmg(0), blockDbk(0), weight(0)
{
}

GFChip::GFChip(int chipNum, int chipColor, int chipClass, int chipType, int chipLevel, int blockAcu, int blockFil,
    int blockDmg, int blockDbk, int weight) :
    chipNum(chipNum), chipColor(chipColor), chipClass(chipClass), chipType(chipType), chipLevel(chipLevel), blockAcu(blockAcu),
    blockFil(blockFil), blockDmg(blockDmg), blockDbk(blockDbk), weight(weight)
{
}


GFChip::~GFChip()
{
}

std::string GFChip::toSaveCode()
{
    string code;
    code += to_string(this->chipNum) + ',';
    code += to_string(this->chipColor) + ',';
    code += to_string(this->chipNum) + ',';
    code += to_string(this->chipType) + ',';
    code += to_string(this->chipLevel) +',';
    code += to_string(this->blockAcu) + ',';
    code += to_string(this->blockFil) + ',';
    code += to_string(this->blockDmg) + ',';
    code += to_string(this->blockDbk) + ',';
    code += to_string(this->weight) + '&';
    return code;
}

std::string GFChip::toExcelLine()
{
    string line;
    return line;
}

void GFChip::fromExcelLine(const std::string& line)
{
}

GFChip GFChip::createFromExcelLine(const std::string& line)
{
    return GFChip();
}

void GFChip::fromSaveCode(const std::string& code)
{
}

GFChip GFChip::createFromSaveCode(const std::string& code)
{
    return GFChip();
}
