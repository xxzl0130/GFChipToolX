#include <iostream>
#include <string>
#include <ctime>
#include "split.h"
#include "GFChip.h"
#include "GFChipSolver.h"
using namespace std;

char buffer[1024000];

int main()
{
    FILE *fin;
    fopen_s(&fin, "SaveCode.txt", "r");
    fread_s(buffer, sizeof(buffer), sizeof(char), sizeof(buffer), fin);
    auto len = strlen(buffer);
    while (buffer[len - 1] == '\n' || buffer[len - 1] == '\r' || buffer[len - 1] == ' ' || buffer[len - 1] < 0)
    {
        //排除末尾空白
        buffer[len - 1] = '\0';
        --len;
    }

    auto list = split(string(buffer + 2, strlen(buffer) - 3 - 2),//skip begin and end
        '&');

    std::vector<GFChip> redChips, blueChips;
    for (const auto& it : list)
    {
        // create chips
        auto chip = GFChip::createFromSaveCode(it);
        if (chip.chipType == 9 || chip.chipType == 10 || chip.chipType > 110 || chip.chipType == 81 || chip.chipType == 82)
        {//5-block-2-type not used in excel
            continue;
        }
        if (chip.chipColor == 2)
        {
            chip.chipNum = redChips.size() + 1;
            redChips.push_back(chip);
        }
        else
        {
            chip.chipNum = blueChips.size() + 1;
            blueChips.push_back(chip);
        }
    }
    auto t0 = clock();
    auto target = Block(16, 10, 7, 3);
    auto solution = solveChip(blueChips, planBGM);
    auto t1 = clock();
    cout << "解法总数：" << solution.size() << endl;
    cout << "耗时：" << double(t1 - t0) / CLOCKS_PER_SEC << "秒" << endl;
    auto index = findSolution(solution, target.blockDmg, target.blockDbk, target.blockAcu, target.blockFil);
    auto t2 = clock();
    cout << "找到最优解" << index.size() << "个" << endl;
    cout << "耗时：" << double(t2 - t1) / CLOCKS_PER_SEC << "秒" << endl;
    for(auto& it : index)
    {
        const auto& s = solution[it];
        for(auto i = 0;i < s.chipNumber;++i)
        {
            cout << s.chipIndex[i] << ",";
        }
        cout << endl;
    }
    cout << endl;
    system("pause");
}