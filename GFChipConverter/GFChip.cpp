#include "GFChip.h"
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>
#include <ctype.h>
#include "split.h"
using namespace std;

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
    if (this->chipClass == 56)
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
        case 11:
            line += "Fa";
            break;
        case 12:
            line += "Fb";
            break;
        case 4:
            line += "T";
            break;
        case 6:
            line += "X";
            break;
        case 31:
            line += "Ya";
            break;
        case 32:
            line += "Yb";
            break;
        case 21:
            line += "Na";
            break;
        case 22:
            line += "Nb";
            break;
        case 5:
            line += "W";
            break;
        case 81:
            line += "b";
            den = den552;
            break;
        case 82:
            line += "d";
            den = den552;
            break;
        case 9:
            line += "I";
            den = den552;
            break;
        case 10:
            line += "C";
            den = den552;
            break;
        case 111:
            line += "Z";
            den = den552;
            break;
        case 112:
            line += "Z-";
            den = den552;
            break;
        case 120:
            line += "V";
            den = den552;
            break;
        case 131:
            line += "L";
            den = den552;
            break;
        case 132:
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
        chip.chipClass = 551;
        // deal with type
        if (list[1] == "fa")
        {
            chip.chipType = 11;
        }
        else if (list[1] == "fb")
        {
            chip.chipType = 12;
        }
        else if (list[1] == "fb")
        {
            chip.chipType = 12;
        }
        else if (list[1] == "t")
        {
            chip.chipType = 4;
        }
        else if (list[1] == "x")
        {
            chip.chipType = 6;
        }
        else if (list[1] == "ya")
        {
            chip.chipType = 31;
        }
        else if (list[1] == "yb")
        {
            chip.chipType = 32;
        }
        else if (list[1] == "na")
        {
            chip.chipType = 21;
        }
        else if (list[1] == "nb")
        {
            chip.chipType = 22;
        }
        else if (list[1] == "w")
        {
            chip.chipType = 5;
        }
        else if (list[1] == "b")
        {
            chip.chipType = 81;
        }
        else if (list[1] == "d")
        {
            chip.chipType = 82;
        }
        else if (list[1] == "i")
        {
            chip.chipType = 9;
        }
        else if (list[1] == "c")
        {
            chip.chipType = 10;
        }
        else if (list[1] == "z")
        {
            chip.chipType = 111;
        }
        else if (list[1] == "z-")
        {
            chip.chipType = 112;
        }
        else if (list[1] == "v")
        {
            chip.chipType = 120;
        }
        else if (list[1] == "l")
        {
            chip.chipType = 131;
        }
        else if (list[1] == "l-")
        {
            chip.chipType = 132;
        }
        else
        {
            return GFChip();
        }
    }
    else
    {
        chip.chipClass = 56;
        // deal with type
        if (list[1] == "4a")
        {
            chip.chipType = 41;
        }
        else if (list[1] == "4b")
        {
            chip.chipType = 42;
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
