#include "GFChip.h"
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>
#include "split.h"
using namespace std;

static constexpr double argDmg = 4.4, argDbk = 12.7, argAcu = 7.1, argFil = 5.7;
static constexpr double den56 = 1.0, den551 = 1.0, den552 = 0.92;
static constexpr double argLv[21] = { 1.0,1.08,1.16,1.24,1.32,1.4,1.48,1.56,1.64,1.72,1.8,1.87,1.94,2.01,2.08,2.15,2.22,2.29,2.36,2.43,2.5 };

string toLower(const string& str)
{
    string ans;
    for(const auto& it : str)
    {
        ans += tolower(it);
    }
    return ans;
}

GFChip::GFChip():chipNum(1), chipColor(1), chipClass(56), chipType(1), chipLevel(0), blockAcu(0),
blockFil(0), blockDmg(6), blockDbk(0), weight(1)
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

std::string GFChip::toSaveCode() const
{
    string code;
    code += to_string(this->chipNum) + ',';
    code += to_string(this->chipColor) + ',';
    code += to_string(this->chipClass) + ',';
    code += to_string(this->chipType) + ',';
    code += to_string(this->chipLevel) +',';
    code += to_string(this->blockAcu) + ',';
    code += to_string(this->blockFil) + ',';
    code += to_string(this->blockDmg) + ',';
    code += to_string(this->blockDbk) + ',';
    code += to_string(this->weight) ;
    return code;
}

void GFChip::fromSaveCode(const std::string& code)
{
    *this = createFromSaveCode(code);
}

GFChip GFChip::createFromSaveCode(const std::string& code)
{
    GFChip chip;
    auto list = split(code, ',');
    if (list.size() < 10)
    {// error, elements not enough
        return GFChip();
    }

    chip.chipNum = atoi(list[0].c_str());
    chip.chipColor = atoi(list[1].c_str());
    chip.chipClass = atoi(list[2].c_str());
    chip.chipType = atoi(list[3].c_str());
    chip.chipLevel = atoi(list[4].c_str());
    if(chip.chipLevel > 20)
    {
        chip.chipLevel = 20;
    }
    else if(chip.chipLevel < 0)
    {
        chip.chipLevel = 0;
    }
    chip.blockAcu = atoi(list[5].c_str());
    chip.blockFil = atoi(list[6].c_str());
    chip.blockDmg = atoi(list[7].c_str());
    chip.blockDbk = atoi(list[8].c_str());
    chip.weight = atoi(list[9].c_str());

    return chip;
}

std::string GFChip::toExcelLine() const
{
    string line;
    line += to_string(this->chipNum) + ',';
    double den = den56;
    if (this->chipClass == GF_CHIP_CLASS_56)
    {
        if(this->chipType > 40)
        {
            //41 to 4a, 42 to 4b
            line += to_string(4) + char('a' + this->chipType - 41) + ',';
        }
        else
        {
            line += to_string(this->chipType) + ',';
        }
    }
    else//551
    {
        den = den551;
        switch(this->chipType)
        {
        case GF_CHIP_TYPE_FA:
            line += "Fa";
            break;
        case GF_CHIP_TYPE_FB:
            line += "Fb";
            break;
        case GF_CHIP_TYPE_T:
            line += "T";
            break;
        case GF_CHIP_TYPE_X:
            line += "X";
            break;
        case GF_CHIP_TYPE_YA:
            line += "Ya";
            break;
        case GF_CHIP_TYPE_YB:
            line += "Yb";
            break;
        case GF_CHIP_TYPE_NA:
            line += "Na";
            break;
        case GF_CHIP_TYPE_NB:
            line += "Nb";
            break;
        case GF_CHIP_TYPE_W:
            line += "W";
            break;
        case GF_CHIP_TYPE_B:
            line += "b";
            den = den552;
            break;
        case GF_CHIP_TYPE_D:
            line += "d";
            den = den552;
            break;
        case GF_CHIP_TYPE_I:
            line += "I";
            den = den552;
            break;
        case GF_CHIP_TYPE_C:
            line += "C";
            den = den552;
            break;
        case GF_CHIP_TYPE_Z:
            line += "Z";
            den = den552;
            break;
        case GF_CHIP_TYPE_Z_:
            line += "Z-";
            den = den552;
            break;
        case GF_CHIP_TYPE_V:
            line += "V";
            den = den552;
            break;
        case GF_CHIP_TYPE_L:
            line += "L";
            den = den552;
            break;
        case GF_CHIP_TYPE_L_:
            line += "L-";
            den = den552;
            break;
        default://error
            return string();
        }
        line += ',';
    }
    line += to_string(this->blockDmg) + ',';
    line += to_string(this->blockDbk) + ',';
    line += to_string(this->blockAcu) + ',';
    line += to_string(this->blockFil) + ',';
    line += to_string(this->chipLevel) + ',';
    line += to_string(ceil(ceil(this->blockDmg * argDmg * den) * argLv[chipLevel])) + ',';
    line += to_string(ceil(ceil(this->blockDbk * argDbk * den) * argLv[chipLevel])) + ',';
    line += to_string(ceil(ceil(this->blockAcu * argAcu * den) * argLv[chipLevel])) + ',';
    line += to_string(ceil(ceil(this->blockFil * argFil * den) * argLv[chipLevel])) + ',';

    return line;
}

void GFChip::fromExcelLine(const std::string& line)
{
    *this = createFromExcelLine(line);
}

GFChip GFChip::createFromExcelLine(const std::string& line)
{
    GFChip chip;
    auto list = split(line,',');
    if (list.size() < 6)
    {// error, elements not enough
        return GFChip();
    }

    chip.chipNum = atoi(list[0].c_str());
    list[1] = toLower(list[1]);
    if(isalpha(list[1][0]))
    {
        // starting with word must be 551
        chip.chipClass = GF_CHIP_CLASS_551;
        // deal with type
        if (list[1] == "fa")
        {
            chip.chipType = GF_CHIP_TYPE_FA;
        }
        else if (list[1] == "fb")
        {
            chip.chipType = GF_CHIP_TYPE_FB;
        }
        else if (list[1] == "t")
        {
            chip.chipType = GF_CHIP_TYPE_T;
        }
        else if (list[1] == "x")
        {
            chip.chipType = GF_CHIP_TYPE_X;
        }
        else if (list[1] == "ya")
        {
            chip.chipType = GF_CHIP_TYPE_YA;
        }
        else if (list[1] == "yb")
        {
            chip.chipType = GF_CHIP_TYPE_YB;
        }
        else if (list[1] == "na")
        {
            chip.chipType = GF_CHIP_TYPE_NA;
        }
        else if (list[1] == "nb")
        {
            chip.chipType = GF_CHIP_TYPE_NB;
        }
        else if (list[1] == "w")
        {
            chip.chipType = GF_CHIP_TYPE_W;
        }
        else if (list[1] == "b")
        {
            chip.chipType = GF_CHIP_TYPE_B;
        }
        else if (list[1] == "d")
        {
            chip.chipType = GF_CHIP_TYPE_D;
        }
        else if (list[1] == "i")
        {
            chip.chipType = GF_CHIP_TYPE_I;
        }
        else if (list[1] == "c")
        {
            chip.chipType = GF_CHIP_TYPE_C;
        }
        else if (list[1] == "z")
        {
            chip.chipType = GF_CHIP_TYPE_Z;
        }
        else if (list[1] == "z-")
        {
            chip.chipType = GF_CHIP_TYPE_Z_;
        }
        else if (list[1] == "v")
        {
            chip.chipType = GF_CHIP_TYPE_V;
        }
        else if (list[1] == "l")
        {
            chip.chipType = GF_CHIP_TYPE_L;
        }
        else if (list[1] == "l-")
        {
            chip.chipType = GF_CHIP_TYPE_L_;
        }
        else
        {
            return GFChip();
        }
    }
    else
    {
        chip.chipClass = GF_CHIP_CLASS_56;
        // deal with type
        if (list[1] == "4a")
        {
            chip.chipType = GF_CHIP_TYPE_4A;
        }
        else if (list[1] == "4b")
        {
            chip.chipType = GF_CHIP_TYPE_4B;
        }
        else
        {
            chip.chipType = atoi(list[1].c_str());
        }
    }
    chip.blockDmg = atoi(list[2].c_str());
    chip.blockDbk = atoi(list[3].c_str());
    chip.blockAcu = atoi(list[4].c_str());
    chip.blockFil = atoi(list[5].c_str());
    if(list.size() > 6)
    {
        chip.chipLevel = atoi(list[6].c_str());
    }
    else
    {
        chip.chipLevel = 0;
    }
    if (chip.chipLevel > 20)
    {
        chip.chipLevel = 20;
    }
    else if (chip.chipLevel < 0)
    {
        chip.chipLevel = 0;
    }
    chip.weight = 1;
    chip.chipColor = 1;//default red.

    return chip;
}
